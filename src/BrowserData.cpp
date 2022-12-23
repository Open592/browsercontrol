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

const std::wstring& BrowserData::GetDestination() const noexcept { return m_destination; }

int BrowserData::GetWidth() const noexcept { return m_size.first; }

int BrowserData::GetHeight() const noexcept { return m_size.second; }

bool BrowserData::IsRunning() const noexcept { return m_state == State::RUNNING; }

void BrowserData::SetDestination(std::wstring&& destination) noexcept
{
    if (destination.empty()) {
        return;
    }

    m_destination = std::move(destination);
}

void BrowserData::SetSize(int width, int height) noexcept { m_size = std::make_pair(width, height); }

void BrowserData::SetState(State state) noexcept
{
    m_state = state;
    m_state.notify_all();
}

/**
 * Report the initialization result after the browser status has transitioned to something other than NOT_STARTED
 */
bool BrowserData::WaitForInitializationResult() noexcept
{
    m_state.wait(State::NOT_STARTED);

    return IsRunning();
}
