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

Base::BrowserContext* Browser::GetBrowserContext() const noexcept { return m_context.get(); }

bool Browser::RegisterBrowserContext(std::unique_ptr<Base::BrowserContext> context) noexcept
{
    if (!context) {
        return false;
    }

    m_context = std::move(context);

    return true;
}
