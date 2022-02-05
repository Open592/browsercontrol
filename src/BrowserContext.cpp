// SPDX-License-Identifier: BSD-2-Clause

#include "BrowserContext.hpp"

bool BrowserContext::RegisterBrowserWindow(std::unique_ptr<AbstractBrowserWindow> window)
{
    if (m_window) {
        // If we attempt to register multiple browser windows with the browser
        // context this is invalid behavior.
        return false;
    }

    m_window = std::move(window);

    return true;
}
