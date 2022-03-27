// SPDX-License-Identifier: BSD-2-Clause

#include "Win32BrowserControl.hpp"
#include "WebView2BrowserWindow.hpp"

[[nodiscard]] HWND Win32BrowserControl::ResolveParentWindow(JNIEnv* env, jobject canvas)
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

    auto winDrawingSurfaceInfo = static_cast<JAWT_Win32DrawingSurfaceInfo*>(drawingSurfaceInfo->platformInfo);

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

DWORD Win32BrowserControl::ThreadProc(LPVOID lpParam)
{
    auto* instance = static_cast<Win32BrowserControl*>(lpParam);

    UINT ret = instance->StartMessagePump();

    return ret;
}

Win32BrowserControl::Win32BrowserControl()
{
    // Initialize the browser window module
    WebView2BrowserWindow::Register();
}

bool Win32BrowserControl::Initialize(JNIEnv* env, jobject canvas, const char* initialDestination) noexcept
{
    if (canvas == nullptr) {
        return false;
    }

    m_parentWindow = ResolveParentWindow(env, canvas);

    if (m_parentWindow == nullptr) {
        return false;
    }

    // Create a manual reset event which will allow the browser window thread to signal when the window was successfully
    // created and will begin accepting messages.
    m_browserWindowCreateEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);

    if (m_browserWindowCreateEvent == nullptr) {
        return false;
    }

    m_browserWindowThread = CreateThread(nullptr, 0, Win32BrowserControl::ThreadProc, this, 0, nullptr);

    if (m_browserWindowThread == nullptr) {
        return false;
    }

    WaitForSingleObject(m_browserWindowCreateEvent, 1000);

    DWORD exitCode;

    if (GetExitCodeThread(m_browserWindowThread, &exitCode) == 0) {
        return false;
    }

    // At this stage we should have initialized the browser window and are beginning to accept window messages. In the
    // case that our window thread is not running we have encountered an error.
    if (exitCode == STILL_ACTIVE) {
        return true;
    }

    // Failed to initialize the browser window
    return false;
}

bool Win32BrowserControl::DeInitialize() noexcept
{
    CloseHandle(m_browserWindowCreateEvent);

    return WebView2BrowserWindow::Unregister();
}

DWORD Win32BrowserControl::StartMessagePump()
{
    m_browserWindow = WebView2BrowserWindow::Create(m_parentWindow, "");

    SetEvent(m_browserWindowCreateEvent);

    if (m_browserWindow == nullptr) {
        return EXIT_FAILURE;
    }

    MSG msg;
    BOOL ret;

    while ((ret = GetMessage(&msg, m_browserWindow, 0, 0)) != 0) {
        if (ret == -1) {
            // Our window quit receiving messages without receiving WM_QUIT. This is an error
            return EXIT_FAILURE;
        } else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return EXIT_SUCCESS;
}

void Win32BrowserControl::Destroy() noexcept
{
    SendMessage(m_browserWindow, EventType::BROWSER_WINDOW_DESTROY, NULL, NULL);
}

void Win32BrowserControl::Resize(int32_t width, int32_t height) noexcept
{
    SendMessage(m_browserWindow, EventType::BROWSER_WINDOW_RESIZE, width, height);
}

void Win32BrowserControl::Navigate(const char* destination) noexcept
{
    SendMessage(m_browserWindow, EventType::BROWSER_WINDOW_NAVIGATE, NULL, reinterpret_cast<LPARAM>(destination));
}
