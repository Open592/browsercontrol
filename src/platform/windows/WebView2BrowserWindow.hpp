// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "../../BrowserData.hpp"

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
    static HWND Create(HWND);

    explicit WebView2BrowserWindow(HWND);

private:
    static constexpr auto WindowClassName = L"Jb";
    static constexpr auto WindowName = L"jbw";

    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
    [[nodiscard]] static std::optional<std::wstring> GetUserDataDirectory() noexcept;

    /**
     * Ensure that the user's machine has a WebView2 runtime installed. In the case
     * that the user does not have one, we attempt to install using the Evergreen
     * Bootstrapper.
     *
     * @return Truthy if the WebView2 runtime is installed or if we successfully
     * installed it.
     */
    [[nodiscard]] static bool EnsureWebViewIsAvailable() noexcept;

    /**
     * Attempt to install the WebView2 runtime using the Evergreen Bootstrapper.
     *
     * @return Truthy if we successfully install the WebView2 runtime.
     */
    [[nodiscard]] static bool InstallWebView() noexcept;

    [[nodiscard]] bool InitializeWebView() noexcept;
    void Destroy();
    void Navigate();
    void Resize();

    HWND m_parentWindow;
    wil::com_ptr<ICoreWebView2Controller> m_controller;
    wil::com_ptr<ICoreWebView2> m_webView;
};
