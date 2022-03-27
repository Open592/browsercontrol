// SPDX-License-Identifier: BSD-2-Clause

#ifndef WEBVIEW2BROWSERWINDOW_H
#define WEBVIEW2BROWSERWINDOW_H

#include <jawt_md.h>
#include <string>

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
    static bool Unregister();
    static HWND Create(HWND, const char*);

    explicit WebView2BrowserWindow(std::string&&);

private:
    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

    void Destroy();
    void Navigate(std::string&&);
    void Resize(int32_t, int32_t);

    std::string m_destination;
};

#endif /* WEBVIEW2BROWSERWINDOW_H */
