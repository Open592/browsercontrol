// SPDX-License-Identifier: BSD-2-Clause

#include <string_view>

class AbstractBrowserWindow {
public:
    AbstractBrowserWindow() = default;

    virtual ~AbstractBrowserWindow() {};

    /**
     * @brief Construct the browser window and perform platform dependent
     * initialization routines to prepare the browser window for interaction.
     *
     * @return Returns truthy on success.
     */
    virtual bool initialize() = 0;

    /**
     * @brief When called we know that we must instruct the native browser
     * to resize to the provided size.
     *
     * @param width
     * @param height
     */
    virtual void resize(int32_t width, int32_t height) = 0;

    /**
     * @brief When called we know that we must instruct the native browser
     * window to navigate to the provided URL.
     *
     * @param toURL
     */
    virtual void navigate(std::string_view toURL) = 0;
};
