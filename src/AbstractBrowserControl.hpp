// SPDX-License-Identifier: BSD-2-Clause

#ifndef ABSTRACTBROWSERCONTROL_H
#define ABSTRACTBROWSERCONTROL_H

#include <string>

class AbstractBrowserControl {
public:
    AbstractBrowserControl() = default;

    virtual ~AbstractBrowserControl() = default;

    /**
     * @brief Initialize the browser window
     *
     * @param env
     * @param parent
     *
     * @return Returns truthy on success.
     */
    virtual bool Initialize(JNIEnv*, jobject, const char*) = 0;

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
    virtual void Resize(int32_t, int32_t) = 0;

    /**
     * @brief Signal the browser window that we must navigate to a new URL
     *
     * @param destination
     */
    virtual void Navigate(const char* destination) = 0;
};

#endif /* ABSTRACTBROWSERCONTROL_H */
