// SPDX-License-Identifier: BSD-2-Clause

#include "LinuxBrowserControl.hpp"
#include "src/BrowserContext.hpp"

__attribute__((constructor)) void setup()
{
    BrowserContext::the().RegisterBrowserControl(std::make_unique<LinuxBrowserControl>());
}

__attribute__((destructor)) void teardown() { BrowserContext::the().UnregisterBrowserControl(); }
