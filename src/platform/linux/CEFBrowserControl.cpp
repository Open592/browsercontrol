// SPDX-License-Identifier: BSD-2-Clause

#include "CEFBrowserControl.hpp"

CEFBrowserControl::CEFBrowserControl() noexcept
    : m_display(DisplayConnection::Connect())
    , m_data(std::make_shared<BrowserData>())
{
    if (m_display == nullptr) {
        // Failed to make a connection to the X server
        m_data->SetState(BrowserData::State::FAILED_TO_START);
    }
}

CEFBrowserControl::~CEFBrowserControl() noexcept = default;

bool CEFBrowserControl::IsRunning() const noexcept { return m_data->IsRunning(); }

bool CEFBrowserControl::Initialize(JNIEnv* env, jobject canvas, std::wstring initialDestination) noexcept {
    CefWindowHandle host = ResolveHostWindow(env, canvas);

    return host != kNullWindowHandle;
}

void CEFBrowserControl::Destroy() noexcept { return; }

void CEFBrowserControl::Resize(int32_t, int32_t) noexcept { return; }

void CEFBrowserControl::Navigate(std::wstring) noexcept { return; }

// static
CefWindowHandle CEFBrowserControl::ResolveHostWindow(JNIEnv* env, jobject canvas) noexcept
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
