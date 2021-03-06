// SPDX-License-Identifier: BSD-2-Clause

#include "BrowserData.hpp"

BrowserData::BrowserData()
    /**
     * Within the initial browsercontrol.dll this is used as the fallback
     * value within the thread proc which initializes the window/browser view
     *
     * Difference:
     * - In the original code this was just "www.jagex.com" (no https)
     */
    : m_destination(L"https://www.jagex.com")
    , m_size(std::make_pair(CW_USEDEFAULT, CW_USEDEFAULT))
    , m_status(Status::NOT_STARTED)
{
}

const std::wstring& BrowserData::GetDestination() const noexcept { return m_destination; }

int BrowserData::GetWidth() const noexcept { return m_size.first; }

int BrowserData::GetHeight() const noexcept { return m_size.second; }

bool BrowserData::IsRunning() const noexcept { return m_status == Status::RUNNING; }

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

void BrowserData::SetStatus(Status status) noexcept
{
    m_status = status;
    m_status.notify_all();
}

/**
 * Report the initialization result after the browser status has transitioned to something other than NOT_STARTED
 */
bool BrowserData::WaitForInitializationResult() noexcept
{
    m_status.wait(Status::NOT_STARTED);

    return IsRunning();
}
