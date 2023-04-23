// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <include/base/cef_ref_counted.h>
#include <include/cef_app.h>

/**
 * Provides the BrowserProcess implementation of CefApp.
 *
 * This is not shared with any other process, we defer to
 * CEF's default implementation for the other processes, and
 * that logic is handled in the helper.
 */
class BrowserApp : public CefApp, public CefBrowserProcessHandler {
public:
    /**
     * Provides a simple interface for classes which will
     * receive events during browser initialization.
     *
     * We are only interested in the `OnContextInitialized` callback
     * from `CefBrowserProcessHandler`.
     */
    struct Delegate {
        /**
         * Once this method has been called we know that CEF
         * has been initialized and we can begin the creation
         * of the browser window.
         */
        virtual void OnContextInitialized() = 0;
    };

    explicit BrowserApp(Delegate&);
    ~BrowserApp() override = default;

    // CefApp overrides
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }

    // CefBrowserProcessHandler overrides
    void OnBeforeCommandLineProcessing(const CefString&, CefRefPtr<CefCommandLine>) override;
    void OnContextInitialized() override;

    IMPLEMENT_REFCOUNTING(BrowserApp);

private:
    Delegate& m_delegate;
};
