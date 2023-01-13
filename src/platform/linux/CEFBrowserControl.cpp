// SPDX-License-Identifier: BSD-2-Clause

#include "CEFBrowserControl.hpp"

CEFBrowserControl::CEFBrowserControl() noexcept
    : m_displayConnection(DisplayConnection::Connect())
{
}

CEFBrowserControl::~CEFBrowserControl() noexcept = default;

bool CEFBrowserControl::IsRunning() const noexcept { return false; }

bool CEFBrowserControl::Initialize(JNIEnv*, jobject, const jchar*) noexcept { return m_displayConnection != nullptr; }

void CEFBrowserControl::Destroy() noexcept { return; }

void CEFBrowserControl::Resize(int32_t, int32_t) noexcept { return; }

void CEFBrowserControl::Navigate(const jchar*) noexcept { return; }
