// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <string>

#include "src/DesktopBrowserLauncher.hpp"

class LinuxDesktopBrowserLauncher final : public DesktopBrowserLauncher {
public:
    LinuxDesktopBrowserLauncher();

    [[nodiscard]] bool IsAvailable() const override;

    bool Open(const std::string&) override;

private:
    std::string m_command;
};
