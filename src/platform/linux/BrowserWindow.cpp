// SPDX-License-Identifier: BSD-2-Clause

#include "BrowserWindow.hpp"

BrowserWindow::BrowserWindow(LinuxBrowserData& data) noexcept
    : m_containerWindow(ContainerWindow(*this))
    , m_data(data)
{
}

bool BrowserWindow::Create()
{
    if (!m_containerWindow.MakeXConnection()) {
        return false;
    }

    if (!m_containerWindow.Create(m_data.GetHostWindow())) {
        return false;
    }

    return true;
}
