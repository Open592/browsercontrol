// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <string>

/**
 * Provides an interface for exposing desktop browsers
 *
 * We pass on URLs to the desktop browser when encountering
 * target="_blank"
 */
class DesktopBrowser {
public:
    /**
     * Allows for checking if we were able to find a
     * supported desktop browser.
     *
     * @return true if we found a browser otherwise false
     */
    virtual bool IsAvailable() = 0;

    /**
     * Opens a URL in the user's default web browser.
     *
     * If possible the URL will be opened in a new tab
     * within running browser sessions - otherwise we
     * will open a new window.
     */
    virtual void Open(const std::string&) = 0;
};
