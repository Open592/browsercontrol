// SPDX-License-Identifier: BSD-2-Clause

#include "src/Browser.hpp"
#include "src/BrowserContext.hpp"

#include "LinuxBrowserContext.hpp"

LinuxBrowserContext::LinuxBrowserContext(std::unique_ptr<LinuxBrowserData> data) noexcept
    : Base::BrowserContext(std::move(data))
{
}

LinuxBrowserData& LinuxBrowserContext::GetBrowserData() const noexcept
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    return static_cast<LinuxBrowserData&>(*m_data);
}

bool LinuxBrowserContext::PerformInitialize(JNIEnv* env, jobject canvas)
{
    std::filesystem::path workingDirectory = m_data->ResolveWorkingDirectory(env);

    if (workingDirectory.empty()) {
        return false;
    }

    return true;
}

void LinuxBrowserContext::PerformDestroy() { }

void LinuxBrowserContext::PerformResize() { }

void LinuxBrowserContext::PerformNavigate() { }
