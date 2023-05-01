// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <include/cef_client.h>

class BrowserHandler : public CefClient, public CefLifeSpanHandler {
public:
    struct Delegate {
        virtual void OnBrowserCreated(CefRefPtr<CefBrowser>) = 0;
        virtual void OnBrowserClosed(CefRefPtr<CefBrowser>) = 0;
    };

    explicit BrowserHandler(Delegate&) noexcept;

    // CefLifeSpanHandler methods
    bool DoClose(CefRefPtr<CefBrowser> browser) override;
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
    void OnAfterCreated(CefRefPtr<CefBrowser>) override;
    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

private:
    Delegate& m_delegate;

    IMPLEMENT_REFCOUNTING(BrowserHandler);
};
