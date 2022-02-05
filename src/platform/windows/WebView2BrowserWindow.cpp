// SPDX-License-Identifier: BSD-2-Clause

#include "WebView2BrowserWindow.hpp"

#define WINDOW_CLASS_NAME "Jb"

/**
 * Initialize window class and setup window event callback listener
 */
WebView2BrowserWindow::WebView2BrowserWindow()
{
    memset(&m_windowClass, 0, sizeof(WNDCLASS));
}

bool WebView2BrowserWindow::Initialize()
{
    return true;
}

void WebView2BrowserWindow::Destroy()
{
}

void WebView2BrowserWindow::HandleResize(int32_t width, int32_t height)
{
}

void WebView2BrowserWindow::HandleNavigate(std::string_view URL)
{
}
