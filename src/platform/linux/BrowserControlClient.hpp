// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <include/cef_client.h>

class BrowserControlClient : public CefClient, public CefContextMenuHandler, public CefLifeSpanHandler {
public:
    static BrowserControlClient* The();

    BrowserControlClient(const BrowserControlClient&) = delete;
    void operator=(const BrowserControlClient&) = delete;

    // CefContextMenuHandler overrides
    CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override { return this; }
    void OnBeforeContextMenu(
        CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, CefRefPtr<CefContextMenuParams>, CefRefPtr<CefMenuModel>) override;
    bool OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame>, const CefString&, const CefString&,
        WindowOpenDisposition, bool, const CefPopupFeatures&, CefWindowInfo&, CefRefPtr<CefClient>&,
        CefBrowserSettings&, CefRefPtr<CefDictionaryValue>&, bool*) override;
    // CefLifeSpanHandler overrides
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
    void OnAfterCreated(CefRefPtr<CefBrowser>) override;

private:
    BrowserControlClient() = default;

    CefRefPtr<CefBrowser> m_browser;

    IMPLEMENT_REFCOUNTING(BrowserControlClient);
};
