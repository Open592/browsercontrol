// SPDX-License-Identifier: BSD-2-Clause

#include "BrowserApp.hpp"

BrowserApp::BrowserApp(BrowserApp::Delegate& delegate)
    : m_delegate(delegate)
{
}

void BrowserApp::OnBeforeCommandLineProcessing(const CefString& processType, CefRefPtr<CefCommandLine> commandLine)
{
    commandLine->AppendSwitch("--disable-extensions");
}

void BrowserApp::OnContextInitialized() { m_delegate.OnContextInitialized(); }
