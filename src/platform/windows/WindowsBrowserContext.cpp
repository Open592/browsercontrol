// SPDX-License-Identifier: BSD-2-Clause

#include <cassert>

#include "src/Browser.hpp"

#include "WebView2BrowserWindow.hpp"
#include "WindowsBrowserContext.hpp"

// static
WindowsBrowserContext* WindowsBrowserContext::The() noexcept
{
    assert(Browser::The().GetBrowserContext() != nullptr);

    // This method would only ever be available when working on Windows
    //
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    return static_cast<WindowsBrowserContext*>(Browser::The().GetBrowserContext());
}

WindowsBrowserContext::WindowsBrowserContext(std::unique_ptr<WindowsBrowserData> data)
    : m_data(std::move(data))
{
}

WindowsBrowserContext::~WindowsBrowserContext() noexcept
{
    if (m_browserThread.joinable()) {
        m_browserThread.join();
    }
}

WindowsBrowserData* WindowsBrowserContext::GetBrowserData() const noexcept { return m_data.get(); }

bool WindowsBrowserContext::PerformInitialize(JNIEnv* env, jobject canvas)
{
    if (canvas == nullptr) {
        return false;
    }

    if (m_data->ResolveHostWindow(env, canvas) == nullptr) {
        return false;
    }

    m_browserThread = std::thread([&] { StartMessagePump(); });

    // Block until the browser reports that we are running
    m_data->WaitForStateOrFailure(Base::ApplicationState::STARTED);

    return m_data->IsRunning();
}

void WindowsBrowserContext::StartMessagePump()
{
    if (!SUCCEEDED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED))) {
        goto handle_error;
    }

    m_browserWindow = WebView2BrowserWindow::Create(m_data->GetHostWindow());

    if (m_browserWindow == nullptr) {
        goto handle_error;
    }

    MSG msg;
    BOOL ret;

    while ((ret = GetMessage(&msg, nullptr, 0, 0)) != 0) {
        if (ret == -1) {
            // Our window quit receiving messages without receiving WM_QUIT. This is an error but at this
            // point we are detached from the original caller, so there is no one to signal this error to.
            return;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return;

handle_error:
    // Notify caller that we failed
    m_data->SetState(Base::ApplicationState::FAILED);
}

void WindowsBrowserContext::PerformDestroy()
{
    SendMessage(m_browserWindow, static_cast<UINT>(WebView2BrowserWindow::EventType::DESTROY), NULL, NULL);
}

void WindowsBrowserContext::PerformResize()
{
    SendMessage(m_browserWindow, static_cast<UINT>(WebView2BrowserWindow::EventType::RESIZE), NULL, NULL);
}

void WindowsBrowserContext::PerformNavigate()
{
    SendMessage(m_browserWindow, static_cast<UINT>(WebView2BrowserWindow::EventType::NAVIGATE), NULL, NULL);
}
