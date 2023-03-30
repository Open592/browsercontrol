// SPDX-License-Identifier: BSD-2-Clause

#include "src/Browser.hpp"
#include "src/BrowserContext.hpp"

#include "LinuxBrowserContext.hpp"

// static
LinuxBrowserContext* LinuxBrowserContext::The()
{
    // This method would only ever be available when working on Windows
    //
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    return static_cast<LinuxBrowserContext*>(Browser::The().GetBrowserContext());
}

LinuxBrowserContext::LinuxBrowserContext(std::unique_ptr<LinuxBrowserData> data) noexcept
    : Base::BrowserContext(std::move(data))
{
}

LinuxBrowserContext::~LinuxBrowserContext() noexcept = default;

LinuxBrowserData* LinuxBrowserContext::GetBrowserData() const noexcept
{
    // This method would only ever be available when working on Windows
    //
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    return static_cast<LinuxBrowserData*>(Base::BrowserContext::GetBrowserData());
}

bool LinuxBrowserContext::PerformInitialize(JNIEnv* env, jobject canvas)
{
    std::filesystem::path workingDirectory = GetBrowserData()->ResolveWorkingDirectory(env);

    if (workingDirectory.empty()) {
        return false;
    }

    return true;
}

void LinuxBrowserContext::PerformDestroy() { }

void LinuxBrowserContext::PerformResize() { }

void LinuxBrowserContext::PerformNavigate() { }
