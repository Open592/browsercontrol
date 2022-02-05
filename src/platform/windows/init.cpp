// SPDX-License-Identifier: BSD-2-Clause

#include <iostream>

#include "process.h"
#include "windows.h"

#include "../../BrowserContext.hpp"
#include "WebView2BrowserWindow.hpp"

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH) {
        std::unique_ptr<WebView2BrowserWindow> window = std::make_unique<WebView2BrowserWindow>();

        if (!BrowserContext::the().RegisterBrowserWindow(std::move(window))) {
            return FALSE;
        }
    }

    return TRUE;
}
