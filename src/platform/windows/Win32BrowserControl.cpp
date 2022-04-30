// SPDX-License-Identifier: BSD-2-Clause

#include "Win32BrowserControl.hpp"
#include "WebView2BrowserWindow.hpp"

HWND Win32BrowserControl::ResolveParentWindow(JNIEnv* env, jobject canvas)
{
    JAWT awt;

    // In the original browsercontrol.dll this was set as JAWT_VERSION_1_4
    awt.version = JAWT_VERSION_9;

    if (JAWT_GetAWT(env, &awt) != JNI_TRUE) {
        return nullptr;
    }

    JAWT_DrawingSurface* drawingSurface = awt.GetDrawingSurface(env, canvas);

    if (drawingSurface == nullptr) {
        return nullptr;
    }

    jint lock = drawingSurface->Lock(drawingSurface);

    if ((lock & JAWT_LOCK_ERROR) != 0) {
        awt.FreeDrawingSurface(drawingSurface);

        return nullptr;
    }

    JAWT_DrawingSurfaceInfo* drawingSurfaceInfo = drawingSurface->GetDrawingSurfaceInfo(drawingSurface);

    if (drawingSurfaceInfo == nullptr) {
        awt.FreeDrawingSurface(drawingSurface);

        return nullptr;
    }

    auto* winDrawingSurfaceInfo = static_cast<JAWT_Win32DrawingSurfaceInfo*>(drawingSurfaceInfo->platformInfo);

    if (winDrawingSurfaceInfo == nullptr) {
        drawingSurface->FreeDrawingSurfaceInfo(drawingSurfaceInfo);
        drawingSurface->Unlock(drawingSurface);
        awt.FreeDrawingSurface(drawingSurface);

        return nullptr;
    }

    HWND result = winDrawingSurfaceInfo->hwnd;

    drawingSurface->FreeDrawingSurfaceInfo(drawingSurfaceInfo);
    drawingSurface->Unlock(drawingSurface);
    awt.FreeDrawingSurface(drawingSurface);

    if (result != nullptr) {
        return result;
    }

    return nullptr;
}

Win32BrowserControl::Win32BrowserControl(std::shared_ptr<BrowserData>&& browserData)
    : m_browserData(std::move(browserData))
    , m_browserWindowCreatedFlag(BrowserWindowCreateStatus::STARTING)
{
    // Initialize the browser window module
    WebView2BrowserWindow::Register();
}

Win32BrowserControl::~Win32BrowserControl() { WebView2BrowserWindow::Unregister(); }

bool Win32BrowserControl::Initialize(JNIEnv* env, jobject canvas, const char* initialDestination) noexcept
{
    if (canvas == nullptr) {
        return false;
    }

    m_parentWindow = ResolveParentWindow(env, canvas);

    if (m_parentWindow == nullptr) {
        return false;
    }

    // On initialization of the browser context a default value is set for the destination.
    // We must update this with the initial destination sent during browsercontrol0()
    m_browserData->SetDestination(initialDestination);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
    // Storing thread on class to allow for separating execution from the invocation of browsercontrol()
    // Will be auto joined either when browsercontrol::destroy0() is called or when the dll is unloaded
    m_browserWindowThread = std::jthread([&] { this->StartMessagePump(); });
#pragma clang diagnostic pop

    // Block until we have received the status of the browser window creation
    m_browserWindowCreatedFlag.wait(BrowserWindowCreateStatus::STARTING);

    if (m_browserWindowCreatedFlag == BrowserWindowCreateStatus::FAILED) {
        // We have failed to create our browser window
        return false;
    }

    return true;
}

void Win32BrowserControl::StartMessagePump()
{
    m_browserWindow = WebView2BrowserWindow::Create(m_parentWindow, m_browserData);

    if (m_browserWindow == nullptr) {
        // Notify the caller that we have failed
        m_browserWindowCreatedFlag = BrowserWindowCreateStatus::FAILED;
        m_browserWindowCreatedFlag.notify_all();

        return;
    }

    // We have successfully created our browser window and are prepared to start accepting messages.
    // At this point browsercontrol0() has fulfilled its duty, and we can signal success back to the caller
    m_browserWindowCreatedFlag = BrowserWindowCreateStatus::SUCCESSFUL;
    m_browserWindowCreatedFlag.notify_all();

    MSG msg;
    BOOL ret;

    while ((ret = GetMessage(&msg, m_browserWindow, 0, 0)) != 0) {
        if (ret == -1) {
            // Our window quit receiving messages without receiving WM_QUIT. This is an error
            return;
        } else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

void Win32BrowserControl::Destroy() noexcept
{
    SendMessage(m_browserWindow, static_cast<UINT>(WebView2BrowserWindow::EventType::DESTROY), NULL, NULL);
}

void Win32BrowserControl::Resize(int32_t width, int32_t height) noexcept
{
    m_browserData->SetWidth(width);
    m_browserData->SetHeight(height);

    SendMessage(m_browserWindow, static_cast<UINT>(WebView2BrowserWindow::EventType::RESIZE), NULL, NULL);
}

void Win32BrowserControl::Navigate(const char* destination) noexcept
{
    m_browserData->SetDestination(destination);

    SendMessage(m_browserWindow, static_cast<UINT>(WebView2BrowserWindow::EventType::NAVIGATE), NULL, NULL);
}
