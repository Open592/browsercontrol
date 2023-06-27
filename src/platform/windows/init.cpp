// SPDX-License-Identifier: BSD-2-Clause

#include "WindowsBrowserContext.hpp"
#include "WindowsBrowserData.hpp"

#include "src/Browser.hpp"

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpReserved)
{
    try {
        if (fdwReason == DLL_PROCESS_ATTACH) {
            auto data = std::make_unique<WindowsBrowserData>();
            auto context = std::make_unique<WindowsBrowserContext>(std::move(data));

            return Browser::The().RegisterContext(std::move(context));
        }

        return true;
    } catch (std::exception&) {
        return false;
    }
}
