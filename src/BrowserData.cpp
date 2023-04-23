// SPDX-License-Identifier: BSD-2-Clause

#include "BrowserData.hpp"

namespace Base {

const std::wstring& BrowserData::GetDestination() const noexcept
{
    std::lock_guard<std::mutex> lk(m_mutex);

    return m_destination;
}

int BrowserData::GetWidth() const noexcept
{
    std::lock_guard<std::mutex> lk(m_mutex);

    return m_size.first;
}

int BrowserData::GetHeight() const noexcept
{
    std::lock_guard<std::mutex> lk(m_mutex);

    return m_size.second;
}

bool BrowserData::IsRunning() const noexcept
{
    std::lock_guard<std::mutex> lk(m_mutex);

    return m_state == ApplicationState::STARTED;
}

void BrowserData::SetDestination(std::wstring destination) noexcept
{
    if (destination.empty()) {
        return;
    }

    std::lock_guard<std::mutex> lk(m_mutex);

    m_destination = std::move(destination);
}

bool BrowserData::SetSize(int width, int height) noexcept
{
    std::lock_guard<std::mutex> lk(m_mutex);

    if (width == m_size.first && height == m_size.second) {
        return false;
    }

    m_size = std::make_pair(width, height);

    return true;
}

void BrowserData::SetState(Base::ApplicationState state) noexcept
{
    {
        std::lock_guard<std::mutex> lk(m_mutex);

        m_state = state;
    }

    m_cv.notify_one();
}

void BrowserData::WaitForStateOrFailure(Base::ApplicationState state) noexcept
{
    std::unique_lock<std::mutex> lk(m_mutex);

    m_cv.wait(lk, [this, state] {
        if (m_state == ApplicationState::FAILED) {
            return true;
        }

        return m_state == state;
    });
}

}
