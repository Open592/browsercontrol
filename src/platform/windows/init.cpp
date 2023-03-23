// SPDX-License-Identifier: BSD-2-Clause

#include "../../BrowserContext.hpp"
#include "Win32BrowserControl.hpp"

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpReserved)
{
    try {
        if (fdwReason == DLL_PROCESS_ATTACH) {
            auto control = std::make_unique<Win32BrowserControl>();
            /**
             * Within the initial browsercontrol.dll this was used as the fallback
             * value within the thread proc which initialized the window/browser view
             */
            auto data = std::make_unique<BrowserData>(CW_USEDEFAULT, CW_USEDEFAULT);

            return BrowserContext::the().RegisterBrowserControl(std::move(control), std::move(data));
        } else if (fdwReason == DLL_PROCESS_DETACH) {
            return BrowserContext::the().UnregisterBrowserControl();
        }

        return TRUE;
    } catch (std::exception&) {
        return FALSE;
    }
}
