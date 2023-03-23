// SPDX-License-Identifier: BSD-2-Clause

#include "LinuxBrowserControl.hpp"
#include "src/BrowserContext.hpp"

__attribute__((constructor)) void setup()
{
    /**
     * CEF requires that we have Xlib support for concurrent threads. Without this
     * enabled we see extremely buggy behavior (for example interaction with sibling
     * windows to CEF's parent window causes the entire application to crash)
     *
     * This is made complicated by the fact that we need to initialize this support
     * before any calls to Xlib are made. For the Applet viewer use-case we can't
     * perform this anywhere in this library because we are not being loaded until
     * after we have already:
     *
     * - Shown the loader box
     * - Closed the loader box
     * - Shown the applet viewer
     * - etc.
     *
     * All of which make calls to the underlying Xlib APIs
     *
     * To solve this we will package a library with the Applet viewer that, for the platforms
     * which require it, invokes XinitThreads on application startup. Once browsercontrol
     * is later loaded, the following call will behave as a no-op. We retain it here purely
     * for convince during development, and any cases which don't have to deal with the above
     * mentioned difficulties.
     *
     * More Info:
     * https://github.com/chromiumembedded/java-cef/blob/ae6912a705e6a2a60f0f156fcc2e8c03bf8285c4/native/context.cpp#L198+L211
     * https://magpcss.org/ceforum/viewtopic.php?f=17&t=16249
     * https://stackoverflow.com/questions/24559368/call-xinitthreads-from-java-gnuplot-error
     */
    XInitThreads();

    auto control = std::make_unique<LinuxBrowserControl>();
    auto data = std::make_unique<BrowserData>();
    BrowserContext::the().RegisterBrowserControl(std::move(control), std::move(data));
}

__attribute__((destructor)) void teardown() { BrowserContext::the().UnregisterBrowserControl(); }
