// SPDX-License-Identifier: BSD-2-Clause

#include "Windows.h"

#include "../../BrowserContext.hpp"
#include "Win32BrowserControl.hpp"

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {
    if (dwReason == DLL_PROCESS_ATTACH) {
        auto control = std::make_unique<Win32BrowserControl>();

        if (!BrowserContext::the().RegisterBrowserControl(std::move(control))) {
            return FALSE;
        }
    } else if (dwReason == DLL_PROCESS_DETACH) {
        if (!BrowserContext::the().UnregisterBrowserControl()) {
            return FALSE;
        }
    }

    return TRUE;
}
