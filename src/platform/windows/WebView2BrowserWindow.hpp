// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "WindowsBrowserData.hpp"

// Custom events which the browser window is listening for
enum class BrowserWindowEvent : unsigned int {
    RESIZE = 0x8001,
    DESTROY = 0x8002,
    NAVIGATE = 0x8003,
};

/**
 * @brief Implementation of a WebView2 browser window for displaying
 * advertisements within the AppletViewer.
 */
class WebView2BrowserWindow {
public:
    static WebView2BrowserWindow* Get(HWND hwnd)
    {
        return reinterpret_cast<WebView2BrowserWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    static HINSTANCE Register();
    static bool Unregister();
    static HWND Create(WindowsBrowserData&);

    explicit WebView2BrowserWindow(HWND, WindowsBrowserData&);

    void Destroy() const;
    void Navigate() const;
    void Resize() const;

private:
    void AddEventHandlers();
    [[nodiscard]] bool InitializeWebView() noexcept;

    HWND m_parentWindow;
    WindowsBrowserData& m_data;
    wil::com_ptr<ICoreWebView2Controller> m_controller;
    wil::com_ptr<ICoreWebView2> m_webView;

    // Event Listener IDs
    EventRegistrationToken m_newWindowRequestToken { 0 };
};
