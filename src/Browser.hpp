// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <memory>

#include "BrowserContext.hpp"
#include "BrowserData.hpp"

/**
 * Provides a singleton reference to the underlying BrowserContext
 *
 * Until we reach the point of accessing platform specific values we
 * will work with the Base classes. Once we transfer control over to
 * the platform specific browser context it will be required to expose
 * a reference to its platform specific implementation of the
 * BrowserContext
 */
class Browser final {
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
    [[nodiscard]] static Browser& The();

    Browser(const Browser&) = delete;
    Browser(Browser&&) = delete;
    Browser& operator=(const Browser&) = delete;
    Browser& operator=(Browser&&) = delete;

    [[nodiscard]] BrowserContext* GetContext() const noexcept;

    bool RegisterContext(std::unique_ptr<BrowserContext>) noexcept;

private:
    Browser();

    std::unique_ptr<BrowserContext> m_context;
};
