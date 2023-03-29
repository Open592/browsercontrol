// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <memory>

#include "BrowserData.hpp"

namespace Base {

/**
 * Provide a generic browser context which exposes a interface for interacting
 * with all of the separate platform's browser controls.
 *
 * It is up to each supported platform to implement the underlying functionality.
 */
class BrowserContext {
public:
    virtual ~BrowserContext() = default;

    [[nodiscard]] virtual BrowserData* GetBrowserData() const noexcept;

    /**
     * @brief Initialize the browser window by passing both the parent container and initial destination.
     *
     * It is the job of the browsercontrol library to fill the parent container with a web view
     *
     * @param parentContainer It is required to pass the raw jobject since obtaining the native window is platform
     * dependent
     * @param initialDestination The browser window will point to this address on initial load. It can be modified
     * using the Navigate method
     *
     * @return Returns true if we were able to successfully initialize the browser control
     */
    bool Initialize(JNIEnv*, jobject, std::wstring);

    void Destroy();

    void Resize(int32_t, int32_t);

    void Navigate(std::wstring);

protected:
    explicit BrowserContext(std::unique_ptr<BrowserData>) noexcept;

    std::unique_ptr<BrowserData> m_data;

private:
    // Each platform is responsible for implementing the API
    //
    // We expect these functions to reference `GetBrowserData` when
    // accessing browser specific data.
    virtual bool PerformInitialize(JNIEnv*, jobject) = 0;
    virtual void PerformDestroy() = 0;
    virtual void PerformResize() = 0;
    virtual void PerformNavigate() = 0;
};

}
