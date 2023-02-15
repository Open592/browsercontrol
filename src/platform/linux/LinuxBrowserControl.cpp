// SPDX-License-Identifier: BSD-2-Clause

#include "LinuxBrowserControl.hpp"

LinuxBrowserControl::LinuxBrowserControl() noexcept
    : m_data(std::make_shared<BrowserData>())
{
}

LinuxBrowserControl::~LinuxBrowserControl() noexcept = default;

bool LinuxBrowserControl::IsRunning() const noexcept { return m_data->IsRunning(); }

bool LinuxBrowserControl::Initialize(JNIEnv* env, jobject canvas, std::wstring initialDestination) noexcept
{
    CefWindowHandle host = ResolveHostWindow(env, canvas);

    if (host == kNullWindowHandle) {
        return false;
    }

    m_data->SetDestination(initialDestination);

    return m_data->WaitForInitializationResult();
}

void LinuxBrowserControl::Destroy() noexcept { return; }

void LinuxBrowserControl::Resize(int32_t, int32_t) noexcept { return; }

void LinuxBrowserControl::Navigate(std::wstring) noexcept { return; }

// static
CefWindowHandle LinuxBrowserControl::ResolveHostWindow(JNIEnv* env, jobject canvas) noexcept
{
    JAWT awt;

    // In the original browsercontrol.dll this was set as JAWT_VERSION_1_4
    awt.version = JAWT_VERSION_9;

    if (JAWT_GetAWT(env, &awt) != JNI_TRUE) {
        return kNullWindowHandle;
    }

    JAWT_DrawingSurface* drawingSurface = awt.GetDrawingSurface(env, canvas);

    if (drawingSurface == nullptr) {
        return kNullWindowHandle;
    }

    jint lock = drawingSurface->Lock(drawingSurface);

    if ((lock & JAWT_LOCK_ERROR) != 0) {
        awt.FreeDrawingSurface(drawingSurface);

        return kNullWindowHandle;
    }

    JAWT_DrawingSurfaceInfo* drawingSurfaceInfo = drawingSurface->GetDrawingSurfaceInfo(drawingSurface);

    if (drawingSurfaceInfo == nullptr) {
        awt.FreeDrawingSurface(drawingSurface);

        return kNullWindowHandle;
    }

    auto* x11DrawingSurfaceInfo = static_cast<JAWT_X11DrawingSurfaceInfo*>(drawingSurfaceInfo->platformInfo);

    if (x11DrawingSurfaceInfo == nullptr) {
        drawingSurface->FreeDrawingSurfaceInfo(drawingSurfaceInfo);
        drawingSurface->Unlock(drawingSurface);
        awt.FreeDrawingSurface(drawingSurface);

        return kNullWindowHandle;
    }

    auto result = static_cast<CefWindowHandle>(x11DrawingSurfaceInfo->drawable);

    drawingSurface->FreeDrawingSurfaceInfo(drawingSurfaceInfo);
    drawingSurface->Unlock(drawingSurface);
    awt.FreeDrawingSurface(drawingSurface);

    return result;
}
