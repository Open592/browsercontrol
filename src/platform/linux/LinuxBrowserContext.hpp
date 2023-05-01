// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <memory>

#include "src/BrowserData.hpp"

#include "BrowserApp.hpp"
#include "BrowserEventLoop.hpp"
#include "BrowserWindow.hpp"
#include "LinuxBrowserData.hpp"

class LinuxBrowserContext final : public Base::BrowserContext,
                                  public BrowserWindow::Delegate,
                                  private BrowserApp::Delegate {
public:
    explicit LinuxBrowserContext(std::unique_ptr<LinuxBrowserData>) noexcept;
    ~LinuxBrowserContext() override = default;

private:
    [[nodiscard]] LinuxBrowserData& GetBrowserData() const noexcept override;

    // BrowserApp::Delegate methods
    void OnContextInitialized() const override;

    // BrowserWindow::Delegate methods
    void OnBrowserWindowDestroyed() override;

    // Platform specific methods
    bool PerformInitialize(JNIEnv*, jobject) override;
    void PerformDestroy() override;
    void PerformResize() override;
    void PerformNavigate() override;

    void StartCEF() const;

    // Store app here since we will exit from PerformInitialize after completion
    //
    // Beyond initialization we aren't going to reference it.
    CefRefPtr<BrowserApp> m_app;
    std::unique_ptr<LinuxBrowserData> m_data;
    std::unique_ptr<BrowserEventLoop> m_eventLoop;
    std::unique_ptr<BrowserWindow> m_browserWindow;
};
