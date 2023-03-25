// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <memory>

#include "src/AbstractBrowserControl.hpp"
#include "src/BrowserData.hpp"

class LinuxBrowserControl : public AbstractBrowserControl {
public:
    LinuxBrowserControl() noexcept = default;

    ~LinuxBrowserControl() override;
    [[nodiscard]] bool Initialize(JNIEnv*, jobject, std::wstring) noexcept override;
    [[nodiscard]] bool IsRunning() const noexcept override;
    void Destroy() noexcept override;
    void Resize(int32_t, int32_t) noexcept override;
    void Navigate(std::wstring) noexcept override;
};
