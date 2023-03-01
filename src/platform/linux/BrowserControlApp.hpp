// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <include/cef_app.h>

#include "../../BrowserData.hpp"

class BrowserControlApp : public CefApp, public CefBrowserProcessHandler {
public:
    explicit BrowserControlApp(std::shared_ptr<BrowserData>, CefWindowHandle) noexcept;

    // CefApp overrides
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }

    // CefBrowserProcessHandler overrides
    void OnBeforeCommandLineProcessing(const CefString&, CefRefPtr<CefCommandLine>) override;
    void OnContextInitialized() override;

private:
    std::shared_ptr<BrowserData> m_data;
    CefWindowHandle m_handle;

    IMPLEMENT_REFCOUNTING(BrowserControlApp);
};
