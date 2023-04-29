// SPDX-License-Identifier: BSD-2-Clause

#include <include/base/cef_bind.h>

#include "BrowserWindow.hpp"

BrowserWindow::BrowserWindow(LinuxBrowserData& data, BrowserEventLoop& eventLoop) noexcept
    : m_clientHandler(new BrowserHandler(*this))
    , m_containerWindow(ContainerWindow(*this))
    , m_eventLoop(eventLoop)
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

void BrowserWindow::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
{
    if (!m_eventLoop.CurrentlyOnBrowserThread()) {
        m_eventLoop.EnqueueWork(base::BindOnce(&BrowserWindow::OnBrowserCreated, base::Unretained(this), browser));

        return;
    }

    if (!m_browser) {
        m_browser = browser;
    }

    m_data.SetState(Base::ApplicationState::STARTED);
}

void BrowserWindow::OnContainerWindowCreate(xcb_window_t containerWindow) const
{
    if (!m_eventLoop.CurrentlyOnBrowserThread()) {
        m_eventLoop.EnqueueWork(
            base::BindOnce(&BrowserWindow::OnContainerWindowCreate, base::Unretained(this), containerWindow));

        return;
    }

    CefBrowserSettings browserSettings;
    CefWindowInfo windowInfo;
    windowInfo.SetAsChild(m_containerWindow.GetWindowHandle(), CefRect(0, 0, m_data.GetWidth(), m_data.GetHeight()));

    bool result = CefBrowserHost::CreateBrowser(
        windowInfo, m_clientHandler.get(), m_data.GetDestination(), browserSettings, nullptr, nullptr);

    if (!result) {
        m_data.SetState(Base::ApplicationState::FAILED);

        return;
    }
}
