// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <filesystem>
#include <memory>
#include <thread>

#include <jawt.h>
#include <jni.h>
#include <xcb/xcb.h>

#include "src/AbstractBrowserControl.hpp"
#include "src/BrowserData.hpp"

#include "CefBrowserProcess.hpp"

class LinuxBrowserControl : public AbstractBrowserControl {
public:
    LinuxBrowserControl() noexcept;

    ~LinuxBrowserControl() override;
    [[nodiscard]] bool Initialize(JNIEnv*, jobject, std::wstring) noexcept override;
    [[nodiscard]] bool IsRunning() const noexcept override;
    void Destroy() noexcept override;
    void Resize(int32_t, int32_t) noexcept override;
    void Navigate(std::wstring) noexcept override;

private:
    /**
     * This represents the host window from the Java parent.
     */
    [[nodiscard]] static xcb_window_t ResolveHostWindow(JNIEnv*, jobject) noexcept;

    /**
     * We require that our host process passes us information about the current working
     * directory of the executable. We use this information to pass the browser subprocess
     * helper path to chromium.
     *
     * This information is passed in Java system properties under the following key:
     *
     * com.open592.workingDirectory
     *
     * We require that the browser helper is a sibling of the applet viewer.
     */
    [[nodiscard]] static std::filesystem::path ResolveWorkingDirectory(JNIEnv*) noexcept;

    /**
     * The parent window is responsible for receiving events and managing size
     *
     * An example of why we need this window:
     *
     * Handling TryCloseBrowser() ->
     *
     * - We receive a request to close
     * - We execute TryCloseBrowser()
     * - Browser cancels the request and executes `onbeforeunload` handler within JS context
     * - Once dialog is closed we receive a close notification to the parent window
     * - We are responsible for responding to this notification and again executing TryCloseBrowser()
     *
     * If we didn't have access to the parent windows events (if we for example used the Java Host window)
     * we wouldn't be able to complete this process - and would need to rely on CloseBrowser(true) or expose
     * some API which the parent could use to manually handle this process.
     */
    [[nodiscard]] xcb_window_t CreateParentWindow(xcb_window_t);
    [[nodiscard]] bool MakeDisplayConnection();

    /**
     * The entry point of the application will be the X window event loop.
     * This will handle all messages from the browsers parent window (which will
     * be parented to the Java host window) - and will also expose and API which
     * will allow us to manually send messages into the browser window.
     */
    [[nodiscard]] bool StartWindowMessageHandler();

    std::shared_ptr<BrowserData> m_data;
    std::thread m_windowMessageLoop;
    std::unique_ptr<CefBrowserProcess> m_browserProcess;

    xcb_connection_t* m_connection;
    xcb_screen_t* m_screen;
    xcb_window_t m_parentWindow;
};
