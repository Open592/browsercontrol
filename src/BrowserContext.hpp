// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <cassert>
#include <jawt.h>
#include <string_view>

#include "AbstractBrowserWindow.h"

/**
 * Event types used in the original browsercontrol.dll
 */
enum class EventType {
    RESIZE = 0x8001,
    DESTROY = 0x8002,
    NAVIGATE = 0x8003,
};

class BrowserContext {
public:
    BrowserContext(AbstractBrowserWindow&& window);

    static BrowserContext& the();
    static void initialize();
    static bool isInitialized();

    void resize(int32_t width, int32_t height)
    {
        assert(BrowserContext::isInitialized());

        m_window->resize(width, height);
    }

    void navigate(std::string_view toURL)
    {
        assert(BrowserContext::isInitialized());

        m_window->navigate(std::move(toURL));
    }

private:
    AbstractBrowserWindow* m_window;
};
