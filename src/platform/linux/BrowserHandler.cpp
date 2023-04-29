// SPDX-License-Identifier: BSD-2-Clause

#include "BrowserHandler.hpp"

BrowserHandler::BrowserHandler(BrowserHandler::Delegate& delegate) noexcept
    : m_delegate(delegate)
{
}

void BrowserHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) { m_delegate.OnBrowserCreated(browser); }

void BrowserHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) { }
