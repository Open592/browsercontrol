// SPDX-License-Identifier: BSD-2-Clause

#include "CEFBrowserControl.hpp"

CEFBrowserControl::CEFBrowserControl() noexcept
    : m_data(std::make_shared<BrowserData>())
    , m_displayConnection(DisplayConnection::Connect())
{
    if (m_displayConnection == nullptr) {
        m_data->SetState(BrowserData::State::FAILED_TO_START);
    }
}

CEFBrowserControl::~CEFBrowserControl() noexcept = default;

bool CEFBrowserControl::IsRunning() const noexcept { return m_data->IsRunning(); }

bool CEFBrowserControl::Initialize(JNIEnv*, jobject, const jchar*) noexcept { return true; }

void CEFBrowserControl::Destroy() noexcept { return; }

void CEFBrowserControl::Resize(int32_t, int32_t) noexcept { return; }

void CEFBrowserControl::Navigate(const jchar*) noexcept { return; }
