// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "src/DesktopBrowser.hpp"

class LinuxDesktopBrowser final : public DesktopBrowser {
public:
    bool IsAvailable() override;

    void Open(const std::string&) override;
};
