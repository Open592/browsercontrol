// SPDX-License-Identifier: BSD-2-Clause

#ifndef BROWSERCONTEXT_H
#define BROWSERCONTEXT_H

#include <cassert>
#include <jawt.h>
#include <memory>
#include <string_view>

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

    bool InitializeBrowserWindow(JNIEnv* env, jobject parentWindow)
    {
        if (!m_control) {
            return false;
        }

        return m_control->Initialize(env, parentWindow);
    }

    void DestroyBrowserWindow()
    {
        if (!m_control) {
            return;
        }

        m_control->Destroy();
    }

    void ResizeBrowserWindow(int32_t width, int32_t height)
    {
        if (!m_control) {
            return;
        }

        m_control->Resize(width, height);
    }

    void NavigateToURL(std::string_view destination)
    {
        if (!m_control || destination.empty()) {
            return;
        }

        m_control->Navigate(destination);
    }

private:
    BrowserContext() = default;

    std::unique_ptr<AbstractBrowserControl> m_control = nullptr;
};

#endif /* BROWSERCONTEXT_H */
