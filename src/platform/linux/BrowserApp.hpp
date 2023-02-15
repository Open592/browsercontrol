// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "include/cef_app.h"

#include "src/BrowserData.hpp"

class BrowserApp : public CefApp, public CefBrowserProcessHandler {
public:
    explicit BrowserApp(std::shared_ptr<BrowserData>, CefWindowHandle) noexcept;

    // CefApp overrides
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }

    // CefBrowserProcessHandler overrides
    void OnContextInitialized() override;
    CefRefPtr<CefClient> GetDefaultClient() override;

private:
    std::shared_ptr<BrowserData> m_browserData;
    CefWindowHandle m_parentWindow;

    IMPLEMENT_REFCOUNTING(BrowserApp);
};
