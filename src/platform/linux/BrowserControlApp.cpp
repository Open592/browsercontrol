// SPDX-License-Identifier: BSD-2-Clause

#include <include/wrapper/cef_helpers.h>

#include "BrowserControlApp.hpp"
#include "BrowserControlClient.hpp"

BrowserControlApp::BrowserControlApp(std::shared_ptr<BrowserData> data, CefWindowHandle handle) noexcept
    : m_data(std::move(data))
    , m_handle(handle)
{
}

BrowserControlApp::~BrowserControlApp() { m_client = nullptr; }

void BrowserControlApp::ShutDown()
{
    auto browser = m_client->GetBrowser();

    if (browser) {
        browser->GetHost()->CloseBrowser(true);
    }
}

void BrowserControlApp::OnBeforeCommandLineProcessing(
    const CefString& processType, CefRefPtr<CefCommandLine> commandLine)
{
    commandLine->AppendSwitch("--disable-extensions");
}

void BrowserControlApp::OnContextInitialized()
{
    CEF_REQUIRE_UI_THREAD();

    CefBrowserSettings browserSettings;

    m_client = new BrowserControlClient(m_data);

    CefWindowInfo windowInfo;

    // Pass the host window (created by Java) to Cef as the parent
    windowInfo.SetAsChild(m_handle, CefRect(0, 0, m_data->GetWidth(), m_data->GetHeight()));

    bool result = CefBrowserHost::CreateBrowser(
        windowInfo, m_client, m_data->GetDestination(), browserSettings, nullptr, nullptr);

    if (!result) {
        m_data->SetState(BrowserData::State::FAILED_TO_START);

        return;
    }
}
