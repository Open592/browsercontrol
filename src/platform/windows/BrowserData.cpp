// SPDX-License-Identifier: BSD-2-Clause

#include "BrowserData.hpp"

BrowserData::BrowserData()
    /**
     * Within the initial browsercontrol.dll this is used as the fallback
     * value within the thread proc which initializes the window/browser view
     *
     * Difference:
     * - In the original code this was "www.jagex.com"
     */
    : m_destination("https://www.jagex.com")
    , m_size(std::make_pair(CW_USEDEFAULT, CW_USEDEFAULT))
{
}

const std::string& BrowserData::GetDestination() const noexcept { return m_destination; }
int BrowserData::GetWidth() const noexcept { return m_size.first; }
int BrowserData::GetHeight() const noexcept { return m_size.second; }

void BrowserData::SetDestination(const char* destination) noexcept
{
    if (destination == nullptr) {
        return;
    }

    m_destination = destination;
}

void BrowserData::SetSize(int width, int height) noexcept { m_size = std::make_pair(width, height); }
