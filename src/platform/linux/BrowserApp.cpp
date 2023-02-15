// SPDX-License-Identifier: BSD-2-Clause

#include "include/wrapper/cef_helpers.h"

#include "BrowserHandler.hpp"

#include "BrowserApp.hpp"

BrowserApp::BrowserApp(std::shared_ptr<BrowserData> browserData, CefWindowHandle parentWindow) noexcept
    : m_browserData(std::move(browserData))
    , m_parentWindow(parentWindow)
{
}

void BrowserApp::OnContextInitialized()
{
    CEF_REQUIRE_UI_THREAD();

    CefBrowserSettings browserSettings;

    CefRefPtr<BrowserHandler> browserHandler = BrowserHandler::The();

    CefWindowInfo windowInfo;

    // TODO: We may need to create our own parent window - currently this is the host from java
    windowInfo.SetAsChild(m_parentWindow, CefRect(0, 0, m_browserData->GetWidth(), m_browserData->GetHeight()));

    bool result = CefBrowserHost::CreateBrowser(
        windowInfo, browserHandler, m_browserData->GetDestination(), browserSettings, nullptr, nullptr);

    if (!result) {
        m_browserData->SetState(BrowserData::State::FAILED_TO_START);

        return;
    }

    m_browserData->SetState(BrowserData::State::RUNNING);
}

CefRefPtr<CefClient> BrowserApp::GetDefaultClient() { return BrowserHandler::The(); }
