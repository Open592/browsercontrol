// SPDX-License-Identifier: BSD-2-Clause

#include "../../BrowserContext.hpp"
#include "BrowserData.hpp"
#include "Win32BrowserControl.hpp"

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    try {
        if (dwReason == DLL_PROCESS_ATTACH) {
            auto control = std::make_unique<Win32BrowserControl>(std::make_shared<BrowserData>());

            return BrowserContext::the().RegisterBrowserControl(std::move(control));
        } else if (dwReason == DLL_PROCESS_DETACH) {
            return BrowserContext::the().UnregisterBrowserControl();
        }

        return TRUE;
    } catch (std::exception&) {
        return FALSE;
    }
}
