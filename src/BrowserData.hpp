// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <condition_variable>
#include <cstdint>
#include <filesystem>
#include <mutex>
#include <string>

#include <jni.h>

namespace Base {

enum class ApplicationState : uint8_t {
    /**
     * Represents the state of:
     * - Waiting to start
     * - Waiting to terminate
     *
     * There is no state to represent "terminated" - this will be
     * represented by the program seizing to exist.
     */
    PENDING,
    /**
     * Represents the state of a browser being ready to navigate to
     * pages.
     */
    STARTED,
    /**
     * Represents any situation where we have failed and are unable to
     * proceed.
     */
    FAILED,
};

class BrowserData {
public:
    [[nodiscard]] const std::wstring& GetDestination() const noexcept;
    [[nodiscard]] int GetWidth() const noexcept;
    [[nodiscard]] int GetHeight() const noexcept;

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
     * Due to this, there may be a small delay for the
     * AppletViewer the first time the user attempts to load the game if they
     * do *not* have WebView2 installed from a previous application.
     *
     * On Linux, we report when we have initialized CEF, but due to CEF requiring
     * a non-zero height/width, we must wait until the subsequent call to `resize()`
     * to perform browser window creation.
     *
     * @return True if we have received signs of life from the browser
     * process.
     */
    [[nodiscard]] bool IsRunning() const noexcept;

    /**
     * Setting the destination does not perform any validation about the
     * validity or security of a particular URL. This is left up to the
     * dll caller as well as the web view.
     */
    void SetDestination(std::wstring) noexcept;
    /**
     * Check if the provided size is different than the current size,
     * if it is store the new size, otherwise do nothing.
     *
     * This is useful due to the resize method being called from the Java
     * side on every resize event, even through the browsercontrol window
     * itself is bounded by strict width/height rules.
     *
     * @return True if we stored a new size, false if we did not.
     */
    [[nodiscard]] bool SetSize(int, int) noexcept;
    void SetState(ApplicationState) noexcept;

    void WaitForStateOrFailure(ApplicationState) noexcept;

protected:
    BrowserData() = default;
    virtual ~BrowserData() = default;

    mutable std::mutex m_mutex;

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
    ApplicationState m_state = ApplicationState::PENDING;

    std::condition_variable m_cv;
};

}
