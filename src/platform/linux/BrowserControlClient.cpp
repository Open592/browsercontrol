// SPDX-License-Identifier: BSD-2-Clause

#include <include/wrapper/cef_helpers.h>

#include "BrowserControlClient.hpp"

BrowserControlClient::BrowserControlClient(std::shared_ptr<BrowserData> data)
    : m_data(std::move(data))
{
}

CefRefPtr<CefBrowser> BrowserControlClient::GetBrowser()
{
    if (m_browser) {
        return m_browser;
    }

    return nullptr;
}

bool BrowserControlClient::DoClose(CefRefPtr<CefBrowser> browser)
{
    // Allow close to continue
    return false;
}

void BrowserControlClient::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
    CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
{
    CEF_REQUIRE_UI_THREAD();

    // Block context menus
    model->Clear();
}

bool BrowserControlClient::OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
    const CefString& target_url, const CefString& target_frame_name, WindowOpenDisposition target_disposition,
    bool user_gesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client,
    CefBrowserSettings& settings, CefRefPtr<CefDictionaryValue>& extra_info, bool* no_javascript_access)
{
    CEF_REQUIRE_UI_THREAD();

    // Block popups
    return true;
}

void BrowserControlClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    if (!m_browser) {
        m_browser = browser;
    }

    m_data->SetState(BrowserData::State::RUNNING);
}

void BrowserControlClient::OnBeforeClose(CefRefPtr<CefBrowser>)
{
    if (m_browser) {
        m_browser = nullptr;
    }

    m_data->SetState(BrowserData::State::TERMINATED);
}
