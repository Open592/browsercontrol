// SPDX-License-Identifier: BSD-2-Clause

#include "BrowserApp.hpp"

void BrowserApp::OnBeforeCommandLineProcessing(const CefString& processType, CefRefPtr<CefCommandLine> commandLine)
{
    commandLine->AppendSwitch("--disable-extensions");
}
