// SPDX-License-Identifier: BSD-2-Clause

#include <jawt.h>
#include <jni.h>

#include "WindowsBrowserData.hpp"

WindowsBrowserData::WindowsBrowserData() noexcept
    : m_host(nullptr)
{
}

HWND WindowsBrowserData::GetHostWindow() const noexcept
{
    std::lock_guard<std::mutex> lk(m_mutex);

    return m_host;
}

HWND WindowsBrowserData::ResolveHostWindow(JNIEnv* env, jobject canvas) noexcept
{
    JAWT awt;

    // NOTE: In the original browsercontrol.dll this was set as JAWT_VERSION_1_4
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

    HWND host = winDrawingSurfaceInfo->hwnd;

    drawingSurface->FreeDrawingSurfaceInfo(drawingSurfaceInfo);
    drawingSurface->Unlock(drawingSurface);
    awt.FreeDrawingSurface(drawingSurface);

    if (host == nullptr) {
        return nullptr;
    }

    std::lock_guard<std::mutex> lk(m_mutex);

    m_host = host;

    return m_host;
}
