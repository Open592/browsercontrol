// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <include/cef_client.h>

#include "LinuxDesktopBrowserLauncher.hpp"

class BrowserHandler : public CefClient,
                       public CefContextMenuHandler,
                       public CefLifeSpanHandler,
                       public CefRequestHandler {
public:
    struct Delegate {
        virtual void OnBrowserCreated(CefRefPtr<CefBrowser>) = 0;
        virtual void OnBrowserClosed(CefRefPtr<CefBrowser>) = 0;
    };

    explicit BrowserHandler(Delegate&) noexcept;

    // CefContextMenuHandler methods
    CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override { return this; }
    void OnBeforeContextMenu(
        CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, CefRefPtr<CefContextMenuParams>, CefRefPtr<CefMenuModel>) override;

    // CefLifeSpanHandler methods
    bool DoClose(CefRefPtr<CefBrowser>) override;
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
    void OnAfterCreated(CefRefPtr<CefBrowser>) override;
    void OnBeforeClose(CefRefPtr<CefBrowser>) override;
    bool OnBeforePopup(CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, const CefString&, const CefString&,
        CefLifeSpanHandler::WindowOpenDisposition, bool, const CefPopupFeatures&, CefWindowInfo&, CefRefPtr<CefClient>&,
        CefBrowserSettings&, CefRefPtr<CefDictionaryValue>&, bool*) override;

    // CefRequestHandler methods
    CefRefPtr<CefRequestHandler> GetRequestHandler() override { return this; }

private:
    Delegate& m_delegate;

    // Provides the ability to open linux within the user's default browser
    std::unique_ptr<LinuxDesktopBrowserLauncher> m_desktopBrowserLauncher;

    IMPLEMENT_REFCOUNTING(BrowserHandler);
};
