// SPDX-License-Identifier: BSD-2-Clause

#include "src/BrowserContext.hpp"

#include "LinuxBrowserControl.hpp"

LinuxBrowserControl::~LinuxBrowserControl() noexcept = default;

bool LinuxBrowserControl::IsRunning() const noexcept { return false; }

bool LinuxBrowserControl::Initialize(JNIEnv* env, jobject canvas, std::wstring initialDestination) noexcept
{
    std::filesystem::path workingDirectory = BrowserContext::the().GetBrowserData()->ResolveWorkingDirectory(env);

    if (workingDirectory.empty()) {
        return false;
    }

    return true;
}

void LinuxBrowserControl::Destroy() noexcept { }

void LinuxBrowserControl::Resize(int32_t, int32_t) noexcept { }

void LinuxBrowserControl::Navigate(std::wstring) noexcept { }
