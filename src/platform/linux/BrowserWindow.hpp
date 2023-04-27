// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "BrowserEventLoop.hpp"
#include "ContainerWindow.hpp"
#include "LinuxBrowserData.hpp"

class BrowserWindow : private ContainerWindow::Delegate {
public:
    explicit BrowserWindow(LinuxBrowserData&) noexcept;

    [[nodiscard]] bool Create();

private:
    ContainerWindow m_containerWindow;
    LinuxBrowserData& m_data;
};
