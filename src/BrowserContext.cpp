// SPDX-License-Identifier: BSD-2-Clause

#include "BrowserContext.hpp"

namespace Base {

BrowserContext::BrowserContext(std::unique_ptr<Base::BrowserData> data) noexcept
    : m_data(std::move(data))
{
    assert(m_data != nullptr && "Expected browser data to exist!");
}

bool BrowserContext::Initialize(JNIEnv* env, jobject parentContainer, std::wstring initialDestination)
{
    // We should not be initializing more than once.
    if (GetBrowserData().IsRunning()) {
        return false;
    }

    if (!initialDestination.empty()) {
        GetBrowserData().SetDestination(std::move(initialDestination));
    }

    return PerformInitialize(env, parentContainer);
}

void BrowserContext::Destroy()
{
    if (!GetBrowserData().IsRunning()) {
        return;
    }

    PerformDestroy();
}

void BrowserContext::Resize(int32_t width, int32_t height)
{
    if (!GetBrowserData().IsRunning()) {
        return;
    }

    GetBrowserData().SetSize(width, height);

    PerformResize();
}

void BrowserContext::Navigate(std::wstring destination)
{
    if (!GetBrowserData().IsRunning() || destination.empty()) {
        return;
    }

    GetBrowserData().SetDestination(std::move(destination));

    PerformNavigate();
}

}
