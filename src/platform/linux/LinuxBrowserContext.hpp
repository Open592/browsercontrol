// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <memory>

#include "src/BrowserData.hpp"

#include "BrowserEventLoop.hpp"
#include "LinuxBrowserData.hpp"

class LinuxBrowserContext : public Base::BrowserContext {
public:
    explicit LinuxBrowserContext(std::unique_ptr<LinuxBrowserData>) noexcept;
    ~LinuxBrowserContext() override = default;

private:
    [[nodiscard]] LinuxBrowserData& GetBrowserData() const noexcept override;

    // Platform specific methods
    bool PerformInitialize(JNIEnv*, jobject) override;
    void PerformDestroy() override;
    void PerformResize() override;
    void PerformNavigate() override;

    std::unique_ptr<LinuxBrowserData> m_data;
    std::unique_ptr<BrowserEventLoop> m_eventLoop;
};
