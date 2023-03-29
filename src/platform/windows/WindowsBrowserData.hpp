// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "src/BrowserData.hpp"

class WindowsBrowserData : public Base::BrowserData {
public:
    WindowsBrowserData() noexcept;
    ~WindowsBrowserData() override = default;

    [[nodiscard]] HWND GetHostWindow() const noexcept;
    [[nodiscard]] HWND ResolveHostWindow(JNIEnv*, jobject) noexcept;

private:
    HWND m_host;
};
