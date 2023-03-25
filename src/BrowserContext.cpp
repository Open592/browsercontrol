// SPDX-License-Identifier: BSD-2-Clause

#include "BrowserContext.hpp"

#include <utility>

bool BrowserContext::RegisterBrowserControl(
    std::unique_ptr<AbstractBrowserControl>&& control, std::unique_ptr<BrowserData>&& data)
{
    if (m_control) {
        // If we attempt to register multiple browser controls with the browser
        // context this is invalid behavior.
        return false;
    }

    m_control = std::move(control);
    m_data = std::move(data);

    return true;
}

bool BrowserContext::UnregisterBrowserControl()
{
    if (!m_control) {
        return false;
    }

    if (m_control->IsRunning()) {
        m_control->Destroy();
    }

    m_control.reset();

    return true;
}

bool BrowserContext::InitializeBrowserWindow(
    JNIEnv* env, jobject parentContainer, std::wstring initialDestination) const noexcept
{
    if (!m_control || m_control->IsRunning()) {
        return false;
    }

    return m_control->Initialize(env, parentContainer, std::move(initialDestination));
}

void BrowserContext::DestroyBrowserWindow() const noexcept
{
    if (!m_control || !m_control->IsRunning()) {
        return;
    }

    m_control->Destroy();
}

void BrowserContext::ResizeBrowserWindow(int32_t width, int32_t height) const noexcept
{
    if (!m_control || !m_control->IsRunning()) {
        return;
    }

    m_control->Resize(width, height);
}

void BrowserContext::Navigate(const std::wstring& destination) const noexcept
{
    if (!m_control || !m_control->IsRunning() || destination.empty()) {
        return;
    }

    m_control->Navigate(destination);
}
