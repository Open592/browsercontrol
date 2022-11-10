// SPDX-License-Identifier: BSD-2-Clause

#include "Win32BrowserControl.hpp"
#include "WebView2BrowserWindow.hpp"

using namespace Microsoft::WRL;

HWND Win32BrowserControl::ResolveHostWindow(JNIEnv* env, jobject canvas)
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

Win32BrowserControl::Win32BrowserControl()
    : m_browserData(std::make_shared<BrowserData>())
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

    m_hostWindow = ResolveHostWindow(env, canvas);

    if (m_hostWindow == nullptr) {
        return false;
    }

    // On initialization of the browser context a default value is set for the destination.
    // We must update this with the initial destination sent during browsercontrol0()
    m_browserData->SetDestination(initialDestination);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
    // Storing thread on class to allow for separating execution from the invocation of browsercontrol()
    // Will be auto joined either when browsercontrol::destroy0() is called or when the dll is unloaded
    m_browserWindowThread = std::jthread([&] { StartMessagePump(); });
#pragma clang diagnostic pop

    return m_browserData->WaitForInitializationResult();
}

bool Win32BrowserControl::IsRunning() const noexcept { return m_browserData->IsRunning(); }

void Win32BrowserControl::StartMessagePump()
{
    if (!SUCCEEDED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED))) {
        goto handle_error;
    }

    m_browserWindow = WebView2BrowserWindow::Create(m_hostWindow, m_browserData);

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
    m_browserData->SetStatus(BrowserData::Status::FAILED_TO_START);
}

void Win32BrowserControl::Destroy() noexcept
{
    SendMessage(m_browserWindow, static_cast<UINT>(WebView2BrowserWindow::EventType::DESTROY), NULL, NULL);
}

void Win32BrowserControl::Resize(int32_t width, int32_t height) noexcept
{
    m_browserData->SetSize(width, height);

    SendMessage(m_browserWindow, static_cast<UINT>(WebView2BrowserWindow::EventType::RESIZE), NULL, NULL);
}

void Win32BrowserControl::Navigate(const char* destination) noexcept
{
    m_browserData->SetDestination(destination);

    SendMessage(m_browserWindow, static_cast<UINT>(WebView2BrowserWindow::EventType::NAVIGATE), NULL, NULL);
}
