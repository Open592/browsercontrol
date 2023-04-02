// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <memory>

#include "src/BrowserData.hpp"

#include "LinuxBrowserData.hpp"

class LinuxBrowserContext : public Base::BrowserContext {
public:
    explicit LinuxBrowserContext(std::unique_ptr<LinuxBrowserData>) noexcept;
    ~LinuxBrowserContext() override = default;

    [[nodiscard]] LinuxBrowserData& GetBrowserData() const noexcept override;

private:
    // Platform specific methods
    bool PerformInitialize(JNIEnv*, jobject) override;
    void PerformDestroy() override;
    void PerformResize() override;
    void PerformNavigate() override;
};
