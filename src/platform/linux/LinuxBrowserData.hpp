// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <filesystem>

#include <jni.h>

#include "src/BrowserData.hpp"

class LinuxBrowserData : public Base::BrowserData {
public:
    [[nodiscard]] std::filesystem::path GetWorkingDirectory() const noexcept { return m_workingDirectory; }
    [[nodiscard]] std::filesystem::path ResolveWorkingDirectory(JNIEnv*) noexcept override;

private:
    /**
     * Refers to the working directory where our CEF dependencies are located.
     *
     * This path also points to where our helper executable is located.
     */
    std::filesystem::path m_workingDirectory;
};
