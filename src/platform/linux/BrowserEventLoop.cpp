// SPDX-License-Identifier: BSD-2-Clause

#include "BrowserEventLoop.hpp"

BrowserEventLoop::BrowserEventLoop()
    : m_isRunning(true)
    , m_thread { &BrowserEventLoop::ThreadWorker, this }
{
}

BrowserEventLoop::~BrowserEventLoop() noexcept
{
    // clang-format off
    //
    // base::Unretained is safe here due to our call to `m_thread.join()` which
    // will block until all outstanding work is completed, thus our this pointer
    // wil continue to be valid until after the call to base::OnceClosure::Run is
    // executed.
    EnqueueWork(base::BindOnce(
        [](BrowserEventLoop* self) { self->m_isRunning = false; },
        base::Unretained(this))
    );
    // clang-format on

    m_thread.join();
}

void BrowserEventLoop::EnqueueWork(base::OnceClosure&& work) noexcept
{
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        m_work.emplace_back(std::move(work));
    }

    m_cv.notify_one();
}

void BrowserEventLoop::ThreadWorker() noexcept
{
    std::vector<base::OnceClosure> currentWork;

    while (m_isRunning) {
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            m_cv.wait(lock, [this] { return !m_work.empty(); });

            std::swap(currentWork, m_work);
        }

        for (base::OnceClosure& work : currentWork) {
            if (work.is_null()) {
                continue;
            }

            std::move(work).Run();
        }

        currentWork.clear();
    }
}
