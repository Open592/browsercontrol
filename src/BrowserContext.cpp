// SPDX-License-Identifier: BSD-2-Clause

#include "BrowserContext.hpp"

bool BrowserContext::RegisterBrowserControl(std::unique_ptr<AbstractBrowserControl>&& control)
{
    if (m_control || !control) {
        // If we attempt to register multiple browser controls with the browser
        // context this is invalid behavior.
        return false;
    }

    m_control = std::move(control);

    return true;
}

bool BrowserContext::UnregisterBrowserControl()
{
    if (!m_control) {
        return false;
    }

    m_control->DeInitialize();
}

bool BrowserContext::InitializeBrowserWindow(
    JNIEnv* env, jobject parentContainer, const char* initialDestination) const noexcept
{
    if (!m_control) {
        return false;
    }

    return m_control->Initialize(env, parentContainer, initialDestination);
}

void BrowserContext::DestroyBrowserWindow() const noexcept
{
    if (!m_control) {
        return;
    }

    m_control->Destroy();
}

void BrowserContext::ResizeBrowserWindow(int32_t width, int32_t height) const noexcept
{
    if (!m_control) {
        return;
    }

    m_control->Resize(width, height);
}

void BrowserContext::Navigate(const char* destination) const noexcept
{
    if (!m_control || destination == nullptr) {
        return;
    }

    m_control->Navigate(destination);
}
