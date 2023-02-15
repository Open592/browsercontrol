// SPDX-License-Identifier: BSD-2-Clause

#include "include/wrapper/cef_helpers.h"

#include "BrowserHandler.hpp"

BrowserHandler* BrowserHandler::The()
{
    static BrowserHandler instance;

    return &instance;
}

void BrowserHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
    CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
{
    CEF_REQUIRE_UI_THREAD();

    // Block context menus
    model->Clear();
}

bool BrowserHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
    const CefString& target_url, const CefString& target_frame_name, WindowOpenDisposition target_disposition,
    bool user_gesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client,
    CefBrowserSettings& settings, CefRefPtr<CefDictionaryValue>& extra_info, bool* no_javascript_access)
{
    CEF_REQUIRE_UI_THREAD();

    // Block popups
    return true;
}

void BrowserHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();

    if (!m_browser) {
        m_browser = browser;
    }
}
