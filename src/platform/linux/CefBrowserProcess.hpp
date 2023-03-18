// SPDX-License-Identifier: BSD-2-Clause

#include <functional>
#include <memory>
#include <mutex>
#include <thread>

#include <include/base/cef_thread_checker.h>
#include <include/cef_base.h>
#include <xcb/xcb.h>

#include "src/BrowserData.hpp"

#include "BrowserControlApp.hpp"

/**
 * This represents the owner of the CEF browser process.
 */
class CefBrowserProcess {
public:
    enum BrowserProcessMessage {
        Create,
        Destroy,
        Terminate,
    };

    explicit CefBrowserProcess(std::shared_ptr<BrowserData>, xcb_window_t, std::filesystem::path) noexcept;
    ~CefBrowserProcess() noexcept;

    CefBrowserProcess(const CefBrowserProcess&) = delete;
    CefBrowserProcess(CefBrowserProcess&&) noexcept = delete;
    CefBrowserProcess& operator=(const CefBrowserProcess&) = delete;
    CefBrowserProcess& operator=(CefBrowserProcess&&) noexcept = delete;

    bool DispatchWindowMessage(const xcb_generic_event_t&);
    void DispatchMessage(BrowserProcessMessage) noexcept;

private:
    void MessageProcessor() noexcept;

    void HandleInitialize();
    void HandleDestroy();

    bool m_isRunning = true;

    std::shared_ptr<BrowserData> m_browserData;
    xcb_window_t m_window;
    std::filesystem::path m_workingDirectory;

    CefRefPtr<BrowserControlApp> m_app;

    std::condition_variable m_conditionVariable;
    std::mutex m_mutex;
    std::thread m_thread;
    base::ThreadChecker m_threadChecker;
    std::vector<BrowserProcessMessage> m_writeBuffer;
};
