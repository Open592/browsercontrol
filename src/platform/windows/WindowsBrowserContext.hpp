// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "src/BrowserContext.hpp"

#include "WindowsBrowserData.hpp"

class WindowsBrowserContext : public Base::BrowserContext {
public:
    /**
     * We require a way to obtain the derived class from within the Windows
     * context. This static method effectively passes through the context
     * pointer provided by the Singleton Browser()
     *
     * For more information about the need for a singleton see Browser::The()
     */
    [[nodiscard]] static WindowsBrowserContext* The() noexcept;

    explicit WindowsBrowserContext(std::unique_ptr<WindowsBrowserData>);
    ~WindowsBrowserContext() noexcept override;

    [[nodiscard]] WindowsBrowserData* GetBrowserData() const noexcept override;

private:
    // Platform specific methods
    bool PerformInitialize(JNIEnv*, jobject) override;
    void PerformDestroy() override;
    void PerformResize() override;
    void PerformNavigate() override;

    void StartMessagePump();

    HWND m_browserWindow = nullptr;

    std::thread m_browserThread;
};
