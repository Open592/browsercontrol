// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <include/cef_client.h>

#include "src/BrowserData.hpp"

class BrowserControlClient : public CefClient, public CefContextMenuHandler, public CefLifeSpanHandler {
public:
    explicit BrowserControlClient(std::shared_ptr<BrowserData>);
    ~BrowserControlClient();

    CefRefPtr<CefBrowser> GetBrowser();

    // CefContextMenuHandler overrides
    CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override { return this; }
    void OnBeforeContextMenu(
        CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, CefRefPtr<CefContextMenuParams>, CefRefPtr<CefMenuModel>) override;
    bool OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame>, const CefString&, const CefString&,
        WindowOpenDisposition, bool, const CefPopupFeatures&, CefWindowInfo&, CefRefPtr<CefClient>&,
        CefBrowserSettings&, CefRefPtr<CefDictionaryValue>&, bool*) override;

    // CefLifeSpanHandler overrides
    bool DoClose(CefRefPtr<CefBrowser> browser) override;
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
    void OnAfterCreated(CefRefPtr<CefBrowser>) override;
    void OnBeforeClose(CefRefPtr<CefBrowser>) override;

private:
    CefRefPtr<CefBrowser> m_browser;
    std::shared_ptr<BrowserData> m_data;

    IMPLEMENT_REFCOUNTING(BrowserControlClient);
};
