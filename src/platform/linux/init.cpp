// SPDX-License-Identifier: BSD-2-Clause

#include "src/BrowserContext.hpp"
#include "CEFBrowserControl.hpp"

__attribute__((constructor)) void setup()
{
    BrowserContext::the().RegisterBrowserControl(std::make_unique<CEFBrowserControl>());
}

__attribute__((destructor)) void teardown()
{
    BrowserContext::the().UnregisterBrowserControl();
}
