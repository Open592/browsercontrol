// SPDX-License-Identifier: BSD-2-Clause

#include "Win32BrowserControl.hpp"
#include "WebView2BrowserWindow.hpp"

Win32BrowserControl::Win32BrowserControl()
{
    // Initialize the browser window
    WebView2BrowserWindow::Register();
}

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

bool Win32BrowserControl::Initialize(JNIEnv* env, jobject canvas, const std::string& initialDestination) noexcept
{
    if (canvas == nullptr) {
        return false;
    }

    HWND parent = ResolveParentWindow(env, canvas);

    if (parent == nullptr) {
        return false;
    }

    HWND browserWindow = WebView2BrowserWindow::Create(parent);

    if (browserWindow == nullptr) {
        return false;
    }

    m_browserWindow = browserWindow;

    return true;
}

void Win32BrowserControl::Destroy() noexcept { }

void Win32BrowserControl::Resize(int32_t width, int32_t height) noexcept
{
    SendMessage(m_browserWindow, EventType::BROWSER_WINDOW_RESIZE, width, height);
}

void Win32BrowserControl::Navigate(const std::string& destination) noexcept
{
    SendMessage(
        m_browserWindow, EventType::BROWSER_WINDOW_NAVIGATE, NULL, reinterpret_cast<LPARAM>(destination.c_str()));
}
