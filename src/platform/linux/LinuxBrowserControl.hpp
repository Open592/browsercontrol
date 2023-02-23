// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <filesystem>
#include <memory>

#include <include/base/cef_thread_checker.h>
#include <include/cef_base.h>
#include <jawt.h>
#include <jni.h>
#include <xcb/xcb.h>

#include "src/AbstractBrowserControl.hpp"
#include "src/BrowserData.hpp"

#include "BrowserControlApp.hpp"

class LinuxBrowserControl : public AbstractBrowserControl {
public:
    LinuxBrowserControl() noexcept;

    ~LinuxBrowserControl() override;
    [[nodiscard]] bool Initialize(JNIEnv*, jobject, std::wstring) noexcept override;
    [[nodiscard]] bool IsRunning() const noexcept override;
    void Destroy() noexcept override;
    void Resize(int32_t, int32_t) noexcept override;
    void Navigate(std::wstring) noexcept override;

private:
    [[nodiscard]] static CefWindowHandle ResolveHostWindow(JNIEnv*, jobject) noexcept;

    /**
     * We require that our host process passes us information about the current working
     * directory of the executable. We use this information to pass the browser subprocess
     * helper path to chromium.
     *
     * This information is passed in Java system properties under the following key:
     *
     * com.open592.workingDirectory
     *
     * We require that the browser helper is a sibling of the applet viewer.
     */
    [[nodiscard]] static std::filesystem::path ResolveWorkingDirectory(JNIEnv*) noexcept;

    CefRefPtr<BrowserControlApp> m_app;
    std::shared_ptr<BrowserData> m_data;
    base::ThreadChecker m_threadChecker;
};
