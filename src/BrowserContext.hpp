// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <cassert>
#include <memory>

#include <jawt.h>
#include <jni.h>

#include "AbstractBrowserControl.hpp"

class BrowserContext {
public:
    /**
     * @brief In order to support the original browsercontrol interface we must
     * store a singleton reference to the browser context.
     *
     * It is unsupported to attempt to initialize more than 1 browser context,
     * and we will fail if attempted.
     *
     * The reason for only supporting a single browsercontext is that the
     * following methods are not sent any reference to the underlying browser
     * window:
     * - destroy
     * - navigate
     * - resize
     *
     * @return Returns singleton reference to BrowserContext
     */
    static BrowserContext& the()
    {
        static BrowserContext instance;

        return instance;
    }

    BrowserContext(const BrowserContext&) = delete;
    void operator=(const BrowserContext&) = delete;

    /**
     * @brief Register the browser control with the browser context.
     *
     * Until we have a valid browser control all other calls to the browser
     * context will fail.
     *
     * It is the job of the shared library initialization code to call this
     * this method.
     *
     * @return Returns truthy if we were able to successfully register the
     * browser control
     */
    bool RegisterBrowserControl(std::unique_ptr<AbstractBrowserControl>&&);

    /**
     * @brief Unregister the browser control
     *
     * This will handle all teardown logic when we are unloading.
     *
     * It is the job of the shared library de-initialization logic to call this method.
     *
     * @return Returns true if we were able to successfully unregister the browser control
     */
    bool UnregisterBrowserControl();

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
    bool InitializeBrowserWindow(JNIEnv*, jobject, std::wstring) const noexcept;

    void DestroyBrowserWindow() const noexcept;

    void ResizeBrowserWindow(int32_t, int32_t) const noexcept;

    void Navigate(std::wstring) const noexcept;

private:
    BrowserContext() = default;

    std::unique_ptr<AbstractBrowserControl> m_control;
};
