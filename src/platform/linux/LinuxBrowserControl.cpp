// SPDX-License-Identifier: BSD-2-Clause

#include "LinuxBrowserControl.hpp"

LinuxBrowserControl::LinuxBrowserControl() noexcept
    : m_data(std::make_shared<BrowserData>())
{
}

LinuxBrowserControl::~LinuxBrowserControl() noexcept = default;

bool LinuxBrowserControl::IsRunning() const noexcept { return false; }

bool LinuxBrowserControl::Initialize(JNIEnv* env, jobject canvas, std::wstring initialDestination) noexcept
{
    return true;
}

void LinuxBrowserControl::Destroy() noexcept { }

void LinuxBrowserControl::Resize(int32_t, int32_t) noexcept { }

void LinuxBrowserControl::Navigate(std::wstring) noexcept { }
