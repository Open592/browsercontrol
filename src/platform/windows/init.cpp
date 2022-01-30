// SPDX-License-Identifier: BSD-2-Clause

#include <iostream>

#include "process.h"
#include "windows.h"

#include "WebView2BrowserWindow.hpp"

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH) {
    }

    return TRUE;
}
