// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <include/cef_browser.h>
#include <include/cef_client.h>

#include "BrowserEventLoop.hpp"
#include "BrowserHandler.hpp"
#include "LinuxBrowserData.hpp"

class BrowserWindow final : private BrowserHandler::Delegate {
public:
    struct Delegate {
        virtual void OnBrowserWindowDestroyed() = 0;
    };

    explicit BrowserWindow(LinuxBrowserData&, BrowserEventLoop&, Delegate&) noexcept;
    ~BrowserWindow() noexcept = default;

    void Create() const;
    void Destroy() const;
    void Navigate() const;
    void Resize() const;

    // BrowserHandler::Delegate methods
    void OnBrowserCreated(CefRefPtr<CefBrowser>) override;
    void OnBrowserClosed(CefRefPtr<CefBrowser>) override;

private:
    CefRefPtr<CefBrowser> m_browser;
    CefRefPtr<CefClient> m_clientHandler;

    LinuxBrowserData& m_data;
    Delegate& m_delegate;
    BrowserEventLoop& m_eventLoop;
};
