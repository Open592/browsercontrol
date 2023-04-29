// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <xcb/xcb.h>

#include <include/cef_browser.h>
#include <include/cef_client.h>

#include "BrowserEventLoop.hpp"
#include "BrowserHandler.hpp"
#include "ContainerWindow.hpp"
#include "LinuxBrowserData.hpp"

class BrowserWindow : private BrowserHandler::Delegate, private ContainerWindow::Delegate {
public:
    explicit BrowserWindow(LinuxBrowserData&, BrowserEventLoop&) noexcept;

    [[nodiscard]] bool Create();

    // BrowserWindow::Delegate methods
    void OnBrowserCreated(CefRefPtr<CefBrowser>) override;
    // ContainerWindow::Delegate methods
    void OnContainerWindowCreate(xcb_window_t) const override;

private:
    CefRefPtr<CefBrowser> m_browser;
    CefRefPtr<CefClient> m_clientHandler;
    ContainerWindow m_containerWindow;
    BrowserEventLoop& m_eventLoop;
    LinuxBrowserData& m_data;
};
