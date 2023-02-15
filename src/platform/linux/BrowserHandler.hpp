// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "include/cef_client.h"

class BrowserHandler : public CefClient, public CefContextMenuHandler, public CefLifeSpanHandler {
public:
    static BrowserHandler* The();

    BrowserHandler(const BrowserHandler&) = delete;
    void operator=(const BrowserHandler&) = delete;

    // CefContextMenuHandler
    CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override { return this; }
    void OnBeforeContextMenu(
        CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, CefRefPtr<CefContextMenuParams>, CefRefPtr<CefMenuModel>) override;
    bool OnBeforePopup(CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, const CefString&, const CefString&,
        WindowOpenDisposition, bool, const CefPopupFeatures&, CefWindowInfo&, CefRefPtr<CefClient>&,
        CefBrowserSettings&, CefRefPtr<CefDictionaryValue>&, bool*) override;

    // CefLifeSpanHandler
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;

private:
    BrowserHandler() = default;

    CefRefPtr<CefBrowser> m_browser = nullptr;

    IMPLEMENT_REFCOUNTING(BrowserHandler);
};
