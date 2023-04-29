// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include <include/base/cef_callback.h>

/**
 * Allows for logic from multiple threads to have the ability to post work to the
 * main application thread.
 *
 * This is required due to CEF having strict requirements that the same thread which
 * called `CefInitialize` is also being used to call other main methods.
 *
 * Also, due to the API exposed by the browsercontrol library, we must utilize the
 * `multi_threaded_message_loop` which forces us to have some way of posting tasks
 * to the thread hosting the message loop.
 */
class BrowserEventLoop {
public:
    BrowserEventLoop();

    BrowserEventLoop(const BrowserEventLoop&) = delete;
    BrowserEventLoop(BrowserEventLoop&&) noexcept = delete;

    BrowserEventLoop& operator=(const BrowserEventLoop&) = delete;
    BrowserEventLoop& operator=(BrowserEventLoop&&) noexcept = delete;

    /**
     * Provide ability to query if the current thread is the main worker
     * thread.
     */
    [[nodiscard]] bool CurrentlyOnBrowserThread() const noexcept;

    /**
     * Enqueue work to be performed on the browser process thread.
     *
     * @example BrowserEventLoop::EnqueueWork(base::BindOnce(...));
     */
    void EnqueueWork(base::OnceClosure&&) noexcept;

private:
    // Allow deletion by std::unique_ptr only
    friend std::default_delete<BrowserEventLoop>;

    ~BrowserEventLoop();

    base::ThreadChecker m_threadChecker;
    std::vector<base::OnceClosure> m_work;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    bool m_isRunning;
    std::thread m_thread;

    void ThreadWorker() noexcept;
};
