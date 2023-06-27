// SPDX-License-Identifier: BSD-2-Clause

#include "Browser.hpp"

// static
Browser& Browser::The()
{
    static Browser instance;

    return instance;
}

Browser::Browser()
    : m_context(nullptr)
{
}

BrowserContext* Browser::GetContext() const noexcept { return m_context.get(); }

bool Browser::RegisterContext(std::unique_ptr<BrowserContext> context) noexcept
{
    if (!context) {
        return false;
    }

    m_context = std::move(context);

    return true;
}
