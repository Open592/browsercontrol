// SPDX-License-Identifier: BSD-2-Clause

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "../../BrowserContext.hpp"
#include "BrowserData.hpp"
#include "Win32BrowserControl.hpp"

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH) {
        try {
            auto control = std::make_unique<Win32BrowserControl>(std::make_shared<BrowserData>());

            return BrowserContext::the().RegisterBrowserControl(std::move(control));
        } catch (std::exception& e) {
            return FALSE;
        }
    } else if (dwReason == DLL_PROCESS_DETACH) {
        return BrowserContext::the().UnregisterBrowserControl();
    }

    return TRUE;
}
