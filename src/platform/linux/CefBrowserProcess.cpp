// SPDX-License-Identifier: BSD-2-Clause

#include <iostream>

#include "CefBrowserProcess.hpp"
#include "JVMSignals.hpp"

CefBrowserProcess::CefBrowserProcess(
    std::shared_ptr<BrowserData> data, xcb_window_t window, std::filesystem::path workingDirectory) noexcept
    : m_browserData(std::move(data))
    , m_window(window)
    , m_workingDirectory(std::move(workingDirectory))
    , m_thread { &CefBrowserProcess::MessageProcessor, this }
{
    // We detach here to allow the first processed message to determine thread ownership
    m_threadChecker.DetachFromThread();
}

CefBrowserProcess::~CefBrowserProcess() noexcept
{
    std::cout << "dror cbp\n";

    DispatchMessage(BrowserProcessMessage::Terminate);

    m_thread.join();
}

bool CefBrowserProcess::DispatchWindowMessage(const xcb_generic_event_t& event)
{
    auto code = event.response_type & ~0x80;

    switch (code) {
    case XCB_MAP_NOTIFY:
        DispatchMessage(BrowserProcessMessage::Create);
        break;
    case XCB_UNMAP_NOTIFY:
        std::cout << "Unmap Notify\n";
        break;
    case XCB_DESTROY_NOTIFY:
        std::cout << "Destroy Notify\n";
        break;
    default:
        std::cout << "Other message code: \n" << code << '\n';
        break;
    }

    return true;
}

void CefBrowserProcess::DispatchMessage(CefBrowserProcess::BrowserProcessMessage message) noexcept
{
    {
        std::lock_guard<std::mutex> guard(m_mutex);

        m_writeBuffer.emplace_back(message);
    }

    m_conditionVariable.notify_one();
}

void CefBrowserProcess::MessageProcessor() noexcept
{
    std::vector<BrowserProcessMessage> readBuffer;

    while (m_isRunning) {
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            m_conditionVariable.wait(lock, [this] { return !m_writeBuffer.empty(); });

            std::swap(readBuffer, m_writeBuffer);
        }

        for (BrowserProcessMessage message : readBuffer) {
            // Determines thread ownership
            //
            // On first invocation we will lock subsequent calls to this thread
            DCHECK(m_threadChecker.CalledOnValidThread());

            switch (message) {
            case Create:
                HandleInitialize();

                break;
            case Destroy:
                HandleDestroy();

                break;
            case Terminate:
                m_isRunning = false;

                break;
            }
        }

        readBuffer.clear();
    }
}

void CefBrowserProcess::HandleInitialize()
{
    auto browserSubProcessHelperPath = m_workingDirectory / "browsercontrol_helper";
    auto debugLogFilePath = m_workingDirectory / "cef_debug.log";

    char* argv[] = { (char*)"browsercontrol" };
    CefMainArgs args(1, argv);

    CefSettings settings;
    CefString(&settings.browser_subprocess_path) = browserSubProcessHelperPath.string();
    settings.multi_threaded_message_loop = true;
    CefString(&settings.log_file) = debugLogFilePath.string();
    CefString(&settings.resources_dir_path) = m_workingDirectory.string();

    m_app = new BrowserControlApp(m_browserData, m_window);

    JVMSignals::Backup();

    bool res = CefInitialize(args, settings, m_app.get(), nullptr);

    if (!res) {
        m_browserData->SetState(BrowserData::State::TERMINATED);

        return;
    }

    JVMSignals::Restore();
}

void CefBrowserProcess::HandleDestroy()
{
    std::cout << "Calling destroy\n";

    m_app->ShutDown();

    m_app.reset();

    CefShutdown();
}
