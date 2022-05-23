// SPDX-License-Identifier: BSD-2-Clause

#ifndef WEBVIEW2BROWSERWINDOW_HPP
#define WEBVIEW2BROWSERWINDOW_HPP

#include "BrowserData.hpp"

/**
 * @brief Implementation of a WebView2 browser window for displaying
 * advertisements within the AppletViewer.
 */
class WebView2BrowserWindow {
public:
    // Window message event types
    enum class EventType : unsigned int {
        RESIZE = 0x8001,
        DESTROY = 0x8002,
        NAVIGATE = 0x8003,
    };

    static WebView2BrowserWindow* Get(HWND hwnd)
    {
        return reinterpret_cast<WebView2BrowserWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    static HINSTANCE Register();
    static bool Unregister();
    static HWND Create(HWND, std::shared_ptr<BrowserData>);

    explicit WebView2BrowserWindow(const std::shared_ptr<BrowserData>*);

private:
    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

    void Destroy();
    void Navigate();
    void Resize();

    const std::shared_ptr<BrowserData> m_data;
};

#endif // WEBVIEW2BROWSERWINDOW_HPP
