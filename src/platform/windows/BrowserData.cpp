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
    : m_destination(L"https://www.jagex.com")
    , m_size(std::make_pair(CW_USEDEFAULT, CW_USEDEFAULT))
{
}

const std::wstring& BrowserData::GetDestination() const noexcept { return m_destination; }
int BrowserData::GetWidth() const noexcept { return m_size.first; }
int BrowserData::GetHeight() const noexcept { return m_size.second; }

void BrowserData::SetDestination(std::string_view destination) noexcept
{
    if (destination.empty()) {
        return;
    }

    size_t size
        = MultiByteToWideChar(CP_UTF8, 0, destination.data(), static_cast<int>(destination.length()), nullptr, 0);
    std::wstring result(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, destination.data(), static_cast<int>(destination.length()), result.data(), size);

    m_destination = result;
}

void BrowserData::SetSize(int width, int height) noexcept { m_size = std::make_pair(width, height); }
