// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <filesystem>

#include <X11/Xlib.h>
#include <jni.h>

#include "src/BrowserData.hpp"

class LinuxBrowserData final : public BrowserData {
public:
    [[nodiscard]] std::filesystem::path GetWorkingDirectory() const noexcept { return m_workingDirectory; }
    [[nodiscard]] Window GetHostWindow() const noexcept { return m_hostWindow; }
    void ResolveWorkingDirectory(JNIEnv*) noexcept;
    [[nodiscard]] bool ResolveHostWindow(JNIEnv*, jobject) noexcept;

private:
    /**
     * Refers to the working directory where our CEF dependencies are located.
     *
     * This path also points to where our helper executable is located.
     */
    std::filesystem::path m_workingDirectory;

    // Handle of the Java host application window
    Window m_hostWindow;
};
