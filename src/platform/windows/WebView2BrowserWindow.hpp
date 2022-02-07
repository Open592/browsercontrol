// SPDX-License-Identifier: BSD-2-Clause

#ifndef WEBVIEW2BROWSERWINDOW_H
#define WEBVIEW2BROWSERWINDOW_H

#include <jawt_md.h>
#include <string_view>

/**
 * Window message event types
 */
enum EventType {
    BROWSER_WINDOW_RESIZE = 0x8001,
    BROWSER_WINDOW_DESTROY = 0x8002,
    BROWSER_WINDOW_NAVIGATE = 0x8003,
};

/**
 * @brief Implementation of a WebView2 browser window for displaying
 * advertisements within the AppletViewer.
 */
class WebView2BrowserWindow {
public:
    static WebView2BrowserWindow* Get(HWND hwnd)
    {
        return reinterpret_cast<WebView2BrowserWindow*>(GetWindowLongPtr(hwnd, 0));
    }
    static HINSTANCE Register();
    static HWND Create(HWND);

    WebView2BrowserWindow() = default;
    ~WebView2BrowserWindow() = default;

private:
    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

    void Destroy();
    void Navigate(std::string_view);
    void Resize(int32_t, int32_t);

    JAWT_Win32DrawingSurfaceInfo* m_drawingSurfaceInfo;
};

#endif /* WEBVIEW2BROWSERWINDOW_H */
