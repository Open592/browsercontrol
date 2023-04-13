// SPDX-License-Identifier: BSD-2-Clause

#include <iostream>

#include "src/BrowserContext.hpp"

#include "LinuxBrowserContext.hpp"

LinuxBrowserContext::LinuxBrowserContext(std::unique_ptr<LinuxBrowserData> data) noexcept
    : m_data(std::move(data))
    , m_eventLoop(std::make_unique<BrowserEventLoop>())
{
    assert(m_data != nullptr && "Expected browser data to exist!");
}

LinuxBrowserData& LinuxBrowserContext::GetBrowserData() const noexcept { return *m_data; }

bool LinuxBrowserContext::PerformInitialize(JNIEnv* env, jobject canvas)
{
    std::filesystem::path workingDirectory = m_data->ResolveWorkingDirectory(env);

    if (workingDirectory.empty()) {
        return false;
    }

    m_eventLoop->EnqueueWork(base::BindOnce([] { std::cout << "Hello world\n"; }));

    return true;
}

void LinuxBrowserContext::PerformDestroy() { }

void LinuxBrowserContext::PerformResize() { }

void LinuxBrowserContext::PerformNavigate() { }
