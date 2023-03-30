// SPDX-License-Identifier: BSD-2-Clause

#include "src/Browser.hpp"
#include "src/BrowserContext.hpp"

#include "LinuxBrowserContext.hpp"

// static
LinuxBrowserContext* LinuxBrowserContext::The()
{
    assert(Browser::The().GetBrowserContext() != nullptr);

    // This method would only ever be available when working on Windows
    //
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    return static_cast<LinuxBrowserContext*>(Browser::The().GetBrowserContext());
}

LinuxBrowserContext::LinuxBrowserContext(std::unique_ptr<LinuxBrowserData> data) noexcept
    : m_data(std::move(data))
{
}

LinuxBrowserData* LinuxBrowserContext::GetBrowserData() const noexcept { return m_data.get(); }

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