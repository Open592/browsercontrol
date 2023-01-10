// SPDX-License-Identifier: BSD-2-Clause

#include <X11/Xlib.h>

#include "src/BrowserContext.hpp"
#include "CEFBrowserControl.hpp"

__attribute__((constructor)) void setup()
{
    if (!XInitThreads()) {
        std::fprintf(stderr, "ERROR (browsercontrol): Failed to initialize X threads.\n");

        exit(EXIT_FAILURE);
    }

    BrowserContext::the().RegisterBrowserControl(std::make_unique<CEFBrowserControl>());
}

__attribute__((destructor)) void teardown()
{
    BrowserContext::the().UnregisterBrowserControl();
}
