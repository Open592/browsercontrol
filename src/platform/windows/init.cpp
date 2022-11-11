// SPDX-License-Identifier: BSD-2-Clause

#include "../../BrowserContext.hpp"
#include "Win32BrowserControl.hpp"

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpReserved)
{
    try {
        if (fdwReason == DLL_PROCESS_ATTACH) {
            return BrowserContext::the().RegisterBrowserControl(std::make_unique<Win32BrowserControl>());
        } else if (fdwReason == DLL_PROCESS_DETACH) {
            return BrowserContext::the().UnregisterBrowserControl();
        }

        return TRUE;
    } catch (std::exception&) {
        return FALSE;
    }
}
