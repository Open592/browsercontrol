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
    , m_width(CW_USEDEFAULT)
    , m_height(CW_USEDEFAULT)
{
}

const std::string& BrowserData::GetDestination() const noexcept { return m_destination; }
int BrowserData::GetWidth() const noexcept { return m_width; }
int BrowserData::GetHeight() const noexcept { return m_height; }

void BrowserData::SetDestination(const char* destination) noexcept
{
    if (destination == nullptr) {
        return;
    }

    m_destination = destination;
}

void BrowserData::SetWidth(int width) noexcept { m_width = width; }
void BrowserData::SetHeight(int height) noexcept { m_height = height; }
