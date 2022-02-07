// SPDX-License-Identifier: BSD-2-Clause

#ifndef ABSTRACTBROWSERWINDOW_H
#define ABSTRACTBROWSERWINDOW_H

#include <string_view>

class AbstractBrowserWindow {
public:
    AbstractBrowserWindow() = default;

    virtual ~AbstractBrowserWindow() {};

    /**
     * @brief Initialize the browser window
     *
     * @return Returns truthy on success.
     */
    virtual bool Initialize() = 0;

    /**
     * @brief Destroy the browser window
     */
    virtual void Destroy() = 0;

    /**
     * @brief Signal the browser window that we must resize
     *
     * @param width
     * @param height
     */
    virtual void Resize(int32_t width, int32_t height) = 0;

    /**
     * @brief Signal the browser window that we must navigate to a new URL
     *
     * @param toURL
     */
    virtual void Navigate(std::string_view toURL) = 0;
};

#endif /* ABSTRACTBROWSERWINDOW_H */
