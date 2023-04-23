// SPDX-License-Identifier: BSD-2-Clause

#include <include/cef_base.h>

#include "BrowserWindow.hpp"

BrowserWindow::BrowserWindow(LinuxBrowserData& data) noexcept
    : m_data(data)
{
}

bool BrowserWindow::CreateHostWindow()
{
    DCHECK(m_displayConnection.MakeConnection());

    return true;
}
