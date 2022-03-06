// SPDX-License-Identifier: BSD-2-Clause

#include "Win32BrowserControl.hpp"
#include "WebView2BrowserWindow.hpp"

Win32BrowserControl::Win32BrowserControl()
{
    // Initialize the browser window
    WebView2BrowserWindow::Register();
}

HWND Win32BrowserControl::ResolveParentWindow(JNIEnv* env, jobject canvas)
{
    JAWT awt;
    JAWT_DrawingSurface* drawingSurface = nullptr;
    JAWT_DrawingSurfaceInfo* drawingSurfaceInfo = nullptr;

    // In the original browsercontrol.dll this was set as JAWT_VERSION_1_4
    awt.version = JAWT_VERSION_9;

    if (JAWT_GetAWT(env, &awt) != JNI_TRUE) {
        return nullptr;
    }

    drawingSurface = awt.GetDrawingSurface(env, canvas);

    if (drawingSurface == nullptr) {
        return nullptr;
    }

    jint lock = drawingSurface->Lock(drawingSurface);

    if ((lock & JAWT_LOCK_ERROR) != 0) {
        goto handle_error_and_return;
    }

    drawingSurfaceInfo = drawingSurface->GetDrawingSurfaceInfo(drawingSurface);

    if (drawingSurfaceInfo == nullptr) {
        goto handle_error_and_return;
    }

    auto winDrawingSurfaceInfo = static_cast<JAWT_Win32DrawingSurfaceInfo*>(drawingSurfaceInfo->platformInfo);

    if (winDrawingSurfaceInfo == nullptr) {
        goto handle_error_and_return;
    }

    HWND result = winDrawingSurfaceInfo->hwnd;

    if (result == nullptr) {
        goto handle_error_and_return;
    }

    drawingSurface->FreeDrawingSurfaceInfo(drawingSurfaceInfo);
    drawingSurface->Unlock(drawingSurface);
    awt.FreeDrawingSurface(drawingSurface);

    return result;

handle_error_and_return:

    if (drawingSurfaceInfo != nullptr) {
        drawingSurface->FreeDrawingSurfaceInfo(drawingSurfaceInfo);

        drawingSurface->Unlock(drawingSurface);
    }

    if (drawingSurface != nullptr) {
        awt.FreeDrawingSurface(drawingSurface);
    }

    return nullptr;
}

bool Win32BrowserControl::Initialize(JNIEnv* env, jobject canvas) noexcept
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

void Win32BrowserControl::Resize(int32_t width, int32_t height) noexcept { }

void Win32BrowserControl::Navigate(std::string_view destination) noexcept { }
