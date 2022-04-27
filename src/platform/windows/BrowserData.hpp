// SPDX-License-Identifier: BSD-2-Clause

#ifndef BROWSERDATA_HPP
#define BROWSERDATA_HPP

#include <string>

class BrowserData {
public:
    /**
     * Initialize BrowserData with a set of default values.
     *
     * These values are generally used as default when calling CreateWindow
     * but I have confirmed that they were used in the original browsercontrol.dll
     */
    BrowserData();

    [[nodiscard]] const std::string& GetDestination() const noexcept;
    [[nodiscard]] int GetWidth() const noexcept;
    [[nodiscard]] int GetHeight() const noexcept;

    /**
     * Setting the destination does not perform any validation about the
     * validity or security of a particular URL. This is left up to the
     * dll caller as well as the web view.
     */
    void SetDestination(const char*) noexcept;

    void SetWidth(int) noexcept;
    void SetHeight(int) noexcept;

private:
    std::string m_destination;

    /**
     * We only need to hold width and height as x,y is always 0.
     *
     * Decompiled example of resize event handler:
     * `SetWindowPos(HWND_18000e1a0,(HWND)0x0,0,0,(int)width,(int)height,0x16);`
     */
    int m_width;
    int m_height;
};

#endif // BROWSERDATA_HPP
