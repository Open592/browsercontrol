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
 *
 * Honestly we don't use CefApp for much here, just some
 * logic within `OnBeforeCommandLineProcessing`
 */
class BrowserApp : public CefApp, public CefBrowserProcessHandler {
public:
    BrowserApp() = default;
    ~BrowserApp() override = default;

    // CefApp overrides
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }

    // CefBrowserProcessHandler overrides
    void OnBeforeCommandLineProcessing(const CefString&, CefRefPtr<CefCommandLine>) override;

    IMPLEMENT_REFCOUNTING(BrowserApp);
};
