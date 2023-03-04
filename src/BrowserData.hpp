// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <condition_variable>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <utility>

class BrowserData {
public:
    enum class State : uint8_t {
        NOT_STARTED,
        RUNNING,
        FAILED_TO_START,
        TERMINATED,
    };

    /**
     * Initialize BrowserData with default width and height
     */
    BrowserData() noexcept;

    /**
     * Initialize BrowserData with an explicit width and height
     */
    explicit BrowserData(int, int) noexcept;

    [[nodiscard]] const std::wstring& GetDestination() noexcept;
    [[nodiscard]] int GetWidth() noexcept;
    [[nodiscard]] int GetHeight() noexcept;

    /**
     * Returns whether or not the browser window is running.
     *
     * This does not mean that the browser window is displaying anything,
     * but is more of a status indicating we have made a request to the
     * underlying browser instance that we would like to be running.
     *
     * In the case of the WebView2 runtime on Windows, if it's not available
     * on the user's machine we must first bootstrap it. During this process
     * this method will continue to report false. We only begin reporting
     * true once we have successfully bootstrapped the WebView2 process.
     *
     * Due to this, on Windows, there may be a small delay for the
     * AppletViewer the first time the user attempts to load the game if they
     * do *not* have WebView2 installed from a previous application.
     *
     * @return True if we have received signs of life from the browser
     * process.
     */
    [[nodiscard]] bool IsRunning() noexcept;
    /**
     * Setting the destination does not perform any validation about the
     * validity or security of a particular URL. This is left up to the
     * dll caller as well as the web view.
     */
    void SetDestination(const std::wstring&) noexcept;
    void SetSize(int, int) noexcept;
    void SetState(State) noexcept;
    void WaitForStateTransition(State) noexcept;
    [[nodiscard]] bool WaitForInitializationResult() noexcept;

private:
    /**
     * Difference: In the original code this was just "www.jagex.com" (no https)
     */
    std::wstring m_destination = L"https://www.jagex.com";

    /**
     * We only need to hold width and height as x,y is always 0.
     *
     * Decompiled example of resize event handler from the Windows DLL:
     * `SetWindowPos(HWND_18000e1a0,(HWND)0x0,0,0,(int)width,(int)height,0x16);`
     */
    std::pair<int, int> m_size;

    /**
     * Tracks the current state of the browser window, allowing for an easy way for exported
     * methods to access the state of the browser control
     */
    State m_state = State::NOT_STARTED;

    std::condition_variable m_cv;
    std::mutex m_mutex;
};
