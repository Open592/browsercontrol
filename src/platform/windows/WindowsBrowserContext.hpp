// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "src/BrowserContext.hpp"

#include "WindowsBrowserData.hpp"

class WindowsBrowserContext : public Base::BrowserContext {
public:
    explicit WindowsBrowserContext(std::unique_ptr<WindowsBrowserData>);
    ~WindowsBrowserContext() noexcept override;

    [[nodiscard]] WindowsBrowserData& GetBrowserData() const noexcept override;

private:
    // Platform specific methods
    bool PerformInitialize(JNIEnv*, jobject) override;
    void PerformDestroy() override;
    void PerformResize() override;
    void PerformNavigate() override;

    void StartMessagePump();

    HWND m_browserWindow = nullptr;

    std::unique_ptr<WindowsBrowserData> m_data;
    std::thread m_browserThread;
};
