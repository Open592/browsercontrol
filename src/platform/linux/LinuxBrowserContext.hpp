// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <memory>

#include "src/BrowserData.hpp"

#include "LinuxBrowserData.hpp"

class LinuxBrowserContext : public Base::BrowserContext {
public:
    /**
     * We require a way to obtain the derived class from within the Windows
     * context. This static method effectively passes through the context
     * pointer provided by the Singleton Browser()
     *
     * For more information about the need for a singleton see Browser::The()
     */
    [[nodiscard]] static LinuxBrowserContext* The();

    explicit LinuxBrowserContext(std::unique_ptr<LinuxBrowserData>) noexcept;
    ~LinuxBrowserContext() override;

    [[nodiscard]] LinuxBrowserData* GetBrowserData() const noexcept override;

private:
    // Platform specific methods
    bool PerformInitialize(JNIEnv*, jobject) override;
    void PerformDestroy() override;
    void PerformResize() override;
    void PerformNavigate() override;
};
