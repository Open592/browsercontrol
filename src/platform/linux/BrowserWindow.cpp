// SPDX-License-Identifier: BSD-2-Clause

#include <include/cef_base.h>

#include "BrowserWindow.hpp"

BrowserWindow::BrowserWindow(LinuxBrowserData& data, BrowserEventLoop& eventLoop) noexcept
    : m_data(data)
    , m_eventLoop(eventLoop) // TODO: Verify we need to pass this
{
}

bool BrowserWindow::CreateHostWindow()
{
    DCHECK(m_eventLoop.CurrentlyOnBrowserThread());
    DCHECK(m_displayConnection.MakeConnection());

    return true;
}
