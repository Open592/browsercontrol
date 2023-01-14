// SPDX-License-Identifier: BSD-2-Clause

#include "BrowserData.hpp"

BrowserData::BrowserData() noexcept
    : m_size(0, 0)
{
}

BrowserData::BrowserData(int width, int height) noexcept
    : m_size(width, height)
{
}

const std::wstring& BrowserData::GetDestination() noexcept
{
    std::lock_guard lk(m_mutex);

    return m_destination;
}

int BrowserData::GetWidth() noexcept
{
    std::lock_guard lk(m_mutex);

    return m_size.first;
}

int BrowserData::GetHeight() noexcept
{
    std::lock_guard lk(m_mutex);

    return m_size.second;
}

bool BrowserData::IsRunning() noexcept
{
    std::lock_guard lk(m_mutex);

    return m_state == State::RUNNING;
}

void BrowserData::SetDestination(std::wstring&& destination) noexcept
{
    if (destination.empty()) {
        return;
    }

    std::lock_guard lk(m_mutex);

    m_destination = std::move(destination);
}

void BrowserData::SetSize(int width, int height) noexcept
{
    std::lock_guard lk(m_mutex);

    m_size = std::make_pair(width, height);
}

void BrowserData::SetState(State state) noexcept
{
    {
        std::lock_guard lk(m_mutex);
        m_state = state;
    }

    m_cv.notify_one();
}

/**
 * Report the initialization result after the browser status has transitioned to something other than NOT_STARTED
 */
bool BrowserData::WaitForInitializationResult() noexcept
{
    std::unique_lock lk(m_mutex);
    m_cv.wait(lk, [this] { return m_state != State::NOT_STARTED; });

    return IsRunning();
}
