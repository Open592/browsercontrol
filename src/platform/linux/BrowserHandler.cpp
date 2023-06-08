// SPDX-License-Identifier: BSD-2-Clause

#include <include/wrapper/cef_helpers.h>

#include "BrowserHandler.hpp"

BrowserHandler::BrowserHandler(BrowserHandler::Delegate& delegate) noexcept
    : m_delegate(delegate)
    , m_desktopBrowser(std::make_unique<LinuxDesktopBrowser>())
{
}

void BrowserHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
    CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
{
    CEF_REQUIRE_UI_THREAD()

    model->Clear();
}

bool BrowserHandler::DoClose(CefRefPtr<CefBrowser> browser) { return false; }

void BrowserHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) { m_delegate.OnBrowserCreated(browser); }

void BrowserHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) { m_delegate.OnBrowserClosed(browser); }

bool BrowserHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
    const CefString& target_url, const CefString& target_frame_name,
    CefLifeSpanHandler::WindowOpenDisposition target_disposition, bool user_gesture,
    const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client,
    CefBrowserSettings& settings, CefRefPtr<CefDictionaryValue>& extra_info, bool* no_javascript_access)
{
    CEF_REQUIRE_UI_THREAD()

    // TODO: Handle links with target="_blank" - they should open in the desktop browser
    if (m_desktopBrowser->IsAvailable()) {
        m_desktopBrowser->Open(target_url);
    }

    // Block popups
    return true;
}
