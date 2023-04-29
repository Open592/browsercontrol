// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <filesystem>

#include <jni.h>
#include <xcb/xcb.h>

#include "src/BrowserData.hpp"

class LinuxBrowserData : public Base::BrowserData {
public:
    [[nodiscard]] std::filesystem::path GetWorkingDirectory() const noexcept { return m_workingDirectory; }
    [[nodiscard]] xcb_window_t GetHostWindow() const noexcept { return m_hostWindow; }
    [[nodiscard]] bool ResolveWorkingDirectory(JNIEnv*) noexcept;
    [[nodiscard]] bool ResolveHostWindow(JNIEnv*, jobject) noexcept;

private:
    /**
     * Refers to the working directory where our CEF dependencies are located.
     *
     * This path also points to where our helper executable is located.
     */
    std::filesystem::path m_workingDirectory;

    // Host window (Java) handle
    xcb_window_t m_hostWindow;
};
