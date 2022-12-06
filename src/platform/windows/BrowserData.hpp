// SPDX-License-Identifier: BSD-2-Clause

#pragma once

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

    /**
     * Returns whether or not the browser window is running.
     *
     * This does not mean that the browser window is displaying anything,
     * but is more of a status indicating we have made a request to the
     * underlying WebView2 instance that we would like to be running.
     *
     * In the case that the WebView2 runtime is not available on the user's
     * machine we must first bootstrap it. During this process we will not
     * report truthy from this method. We only begin to return truthy once
     * we have successfully bootstrapped the WebView2 process.
     *
     * Due to this there may be a small delay on the AppletViewer side the
     * first time users attempt to load the game if they do *not* have
     * WebView2 installed from previous applications.
     *
     * @return Truthy if we have received signs of life from the WebView2
     * process.
     */
    [[nodiscard]] bool IsRunning() const noexcept;
    /**
     * Setting the destination does not perform any validation about the
     * validity or security of a particular URL. This is left up to the
     * dll caller as well as the web view.
     */
    void SetDestination(std::wstring&&) noexcept;
    void SetSize(int, int) noexcept;
    void SetStatus(Status) noexcept;
    [[nodiscard]] bool WaitForInitializationResult() noexcept;

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
