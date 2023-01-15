// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "../../AbstractBrowserControl.hpp"
#include "../../BrowserData.hpp"

class Win32BrowserControl : public AbstractBrowserControl {
public:
    Win32BrowserControl();

    ~Win32BrowserControl() override;
    bool Initialize(JNIEnv*, jobject, std::wstring) noexcept override;
    [[nodiscard]] bool IsRunning() const noexcept override;
    void Destroy() noexcept override;
    void Resize(int32_t, int32_t) noexcept override;
    void Navigate(std::wstring) noexcept override;

private:
    [[nodiscard]] static HWND ResolveHostWindow(JNIEnv*, jobject);

    void StartMessagePump();

    HWND m_browserWindow = nullptr;
    HWND m_hostWindow = nullptr;

    std::shared_ptr<BrowserData> m_browserData;
    [[maybe_unused]] std::jthread m_browserWindowThread;
};
