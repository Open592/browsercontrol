// SPDX-License-Identifier: BSD-2-Clause

#include "BrowserContext.hpp"

namespace Base {

bool BrowserContext::Initialize(JNIEnv* env, jobject parentContainer, std::wstring initialDestination)
{
    // We should not be initializing more than once.
    if (!GetBrowserData() || GetBrowserData()->IsRunning()) {
        return false;
    }

    if (!initialDestination.empty()) {
        GetBrowserData()->SetDestination(std::move(initialDestination));
    }

    return PerformInitialize(env, parentContainer);
}

void BrowserContext::Destroy()
{
    if (!GetBrowserData() || !GetBrowserData()->IsRunning()) {
        return;
    }

    PerformDestroy();
}

void BrowserContext::Resize(int32_t width, int32_t height)
{
    if (!GetBrowserData() || !GetBrowserData()->IsRunning()) {
        return;
    }

    GetBrowserData()->SetSize(width, height);

    PerformResize();
}

void BrowserContext::Navigate(std::wstring destination)
{
    if (!GetBrowserData() || !GetBrowserData()->IsRunning() || destination.empty()) {
        return;
    }

    GetBrowserData()->SetDestination(std::move(destination));

    PerformNavigate();
}

}
