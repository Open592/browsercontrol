// SPDX-License-Identifier: BSD-2-Clause

#ifndef BROWSERDATA_HPP
#define BROWSERDATA_HPP

class BrowserData {
public:
    enum class Status : uint8_t {
        NOT_STARTED,
        RUNNING,
        FAILED_TO_START,
        TERMINATED,
    };

    /**
     * Initialize BrowserData with a set of default values.
     *
     * These values are generally used as default when calling CreateWindow
     * but I have confirmed that they were used in the original browsercontrol.dll
     */
    BrowserData();

    [[nodiscard]] const std::wstring& GetDestination() const noexcept;
    [[nodiscard]] int GetWidth() const noexcept;
    [[nodiscard]] int GetHeight() const noexcept;
    [[nodiscard]] bool IsRunning() const noexcept;
    /**
     * Setting the destination does not perform any validation about the
     * validity or security of a particular URL. This is left up to the
     * dll caller as well as the web view.
     */
    void SetDestination(std::string_view) noexcept;
    void SetSize(int, int) noexcept;
    void SetStatus(Status) noexcept;
    void WaitForInitializationResult() noexcept;

private:
    std::wstring m_destination;

    /**
     * We only need to hold width and height as x,y is always 0.
     *
     * Decompiled example of resize event handler:
     * `SetWindowPos(HWND_18000e1a0,(HWND)0x0,0,0,(int)width,(int)height,0x16);`
     */
    std::pair<int, int> m_size;

    /**
     * Tracks the current status of the browser window, allowing for an easy way for exported
     * methods to access the state of the browser control
     */
    std::atomic<Status> m_status;
};

#endif // BROWSERDATA_HPP
