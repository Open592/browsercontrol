// SPDX-License-Identifier: BSD-2-Clause

#ifndef WEBVIEW2BROWSERWINDOW_H
#define WEBVIEW2BROWSERWINDOW_H

#include <jawt.h>
#include <jawt_md.h>
#include <jni.h>

#include "../../AbstractBrowserWindow.hpp"

/**
 * Window message event types
 */
enum class EventType : UINT {
    RESIZE = 0x8001,
    DESTROY = 0x8002,
    NAVIGATE = 0x8003,
};

/**
 * @brief Implementation of a WebView2 browser window for displaying
 * advertisements within the AppletViewer.
 */
class WebView2BrowserWindow : public AbstractBrowserWindow {
public:
    static JAWT_Win32DrawingSurfaceInfo* ResolveDrawingSurfaceInfo(JNIEnv*, jobject);

    WebView2BrowserWindow();
    ~WebView2BrowserWindow() = default;

    virtual bool Initialize() override;
    virtual void Destroy() override;
    virtual void HandleResize(int32_t, int32_t) override;
    virtual void HandleNavigate(std::string_view) override;

private:
    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

    WNDCLASS m_windowClass;
    JAWT_Win32DrawingSurfaceInfo* m_drawingSurfaceInfo;
};

#endif /* WEBVIEW2BROWSERWINDOW_H */
