// SPDX-License-Identifier: BSD-2-Clause

#include "BrowserContext.hpp"

bool BrowserContext::RegisterBrowserControl(std::unique_ptr<AbstractBrowserControl>&& control)
{
    if (m_control || !control) {
        // If we attempt to register multiple browser controls with the browser
        // context this is invalid behavior.
        return false;
    }

    m_control = std::move(control);

    return true;
}
