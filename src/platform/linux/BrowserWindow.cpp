// SPDX-License-Identifier: BSD-2-Clause

#include <X11/Xlib.h>
#include <include/base/cef_bind.h>
#include <include/wrapper/cef_closure_task.h>

#include "BrowserWindow.hpp"

BrowserWindow::BrowserWindow(LinuxBrowserData& data, BrowserEventLoop& eventLoop, Delegate& delegate) noexcept
    : m_clientHandler(new BrowserHandler(*this))
    , m_data(data)
    , m_delegate(delegate)
    , m_eventLoop(eventLoop)
{
}

void BrowserWindow::Create() const
{
    if (!m_eventLoop.CurrentlyOnBrowserThread()) {
        return m_eventLoop.EnqueueWork(base::BindOnce(&BrowserWindow::Create, base::Unretained(this)));
    }

    CefBrowserSettings browserSettings;
    CefWindowInfo windowInfo;
    windowInfo.SetAsChild(m_data.GetHostWindow(), CefRect(0, 0, m_data.GetWidth(), m_data.GetHeight()));

    bool result = CefBrowserHost::CreateBrowser(
        windowInfo, m_clientHandler.get(), m_data.GetDestination(), browserSettings, nullptr, nullptr);

    if (!result) {
        m_data.SetState(ApplicationState::FAILED);

        return;
    }
}

void BrowserWindow::Destroy() const
{
    if (!CefCurrentlyOn(TID_UI)) {
        CefPostTask(TID_UI, base::BindOnce(&BrowserWindow::Destroy, base::Unretained(this)));

        return;
    }

    if (m_browser) {
        m_browser->GetHost()->TryCloseBrowser();
    }
}

void BrowserWindow::Navigate() const
{
    if (m_browser) {
        m_browser->GetMainFrame()->LoadURL(m_data.GetDestination());
    }
}

void BrowserWindow::Resize() const
{
    /**
     * Required to run on `TID_UI` so we can access `cef_get_xdisplay`
     */
    if (!CefCurrentlyOn(TID_UI)) {
        CefPostTask(TID_UI, base::BindOnce(&BrowserWindow::Resize, base::Unretained(this)));

        return;
    }

    if (!m_browser) {
        return;
    }

    auto display = cef_get_xdisplay();
    auto handle = m_browser->GetHost()->GetWindowHandle();
    auto window = static_cast<Window>(handle);

    XMoveResizeWindow(display, window, 0, 0, m_data.GetWidth(), m_data.GetHeight());
    XFlush(display);
}

void BrowserWindow::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
{
    if (!m_browser) {
        m_browser = browser;
    }

    m_data.SetState(ApplicationState::STARTED);
}

void BrowserWindow::OnBrowserClosed(CefRefPtr<CefBrowser> browser)
{
    if (!browser) {
        return;
    }

    DCHECK(m_browser->IsSame(browser));

    m_browser = nullptr;

    m_delegate.OnBrowserWindowDestroyed();
}
