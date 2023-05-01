// SPDX-License-Identifier: BSD-2-Clause

#include <include/base/cef_callback.h>
#include <include/cef_base.h>
#include <include/cef_task.h>
#include <include/wrapper/cef_closure_task.h>

#include "src/BrowserContext.hpp"

#include "BrowserApp.hpp"
#include "JVMSignals.hpp"
#include "LinuxBrowserContext.hpp"

LinuxBrowserContext::LinuxBrowserContext(std::unique_ptr<LinuxBrowserData> data) noexcept
    : m_app(new BrowserApp(*this))
    , m_data(std::move(data))
    , m_eventLoop(std::make_unique<BrowserEventLoop>())
{
    assert(m_data != nullptr && "Expected browser data to exist!");
    assert(m_eventLoop != nullptr && "Expected browser event loop to exist!");

    m_browserWindow = std::make_unique<BrowserWindow>(*m_data, *m_eventLoop, *this);
}

LinuxBrowserData& LinuxBrowserContext::GetBrowserData() const noexcept { return *m_data; }

void LinuxBrowserContext::OnContextInitialized() const
{
    if (!m_eventLoop->CurrentlyOnBrowserThread()) {
        m_eventLoop->EnqueueWork(base::BindOnce(&LinuxBrowserContext::OnContextInitialized, base::Unretained(this)));

        return;
    }

    m_browserWindow->Create();
}

void LinuxBrowserContext::OnBrowserWindowDestroyed()
{
    if (!m_eventLoop->CurrentlyOnBrowserThread()) {
        m_eventLoop->EnqueueWork(
            base::BindOnce(&LinuxBrowserContext::OnBrowserWindowDestroyed, base::Unretained(this)));

        return;
    }

    m_browserWindow.reset();

    CefShutdown();

    m_data->SetState(Base::ApplicationState::PENDING);
}

bool LinuxBrowserContext::PerformInitialize(JNIEnv* env, jobject canvas)
{
    if (!m_data->ResolveWorkingDirectory(env)) {
        return false;
    }

    if (!m_data->ResolveHostWindow(env, canvas)) {
        return false;
    }

    m_eventLoop->EnqueueWork(base::BindOnce(&LinuxBrowserContext::StartCEF, base::Unretained(this)));

    m_data->WaitForStateOrFailure(Base::ApplicationState::STARTED);

    return m_data->IsRunning();
}

void LinuxBrowserContext::PerformDestroy()
{
    if (!CefCurrentlyOn(TID_UI)) {
        CefPostTask(TID_UI,
            base::BindOnce([](BrowserWindow* window) { window->Destroy(); }, base::Unretained(m_browserWindow.get())));

        m_data->WaitForStateOrFailure(Base::ApplicationState::PENDING);
    } else {
        m_browserWindow->Destroy();

        m_data->WaitForStateOrFailure(Base::ApplicationState::PENDING);
    }
}

void LinuxBrowserContext::PerformResize() { }

void LinuxBrowserContext::PerformNavigate() { m_browserWindow->Navigate(); }

void LinuxBrowserContext::StartCEF() const
{
    DCHECK(m_eventLoop->CurrentlyOnBrowserThread());

    auto subProcessHelperPath = m_data->GetWorkingDirectory() / "browsercontrol_helper";
    auto cefDebugLogPath = m_data->GetWorkingDirectory() / "cef_debug.log";

    char* argv[] = { const_cast<char*>("browsercontrol") };
    CefMainArgs args(1, argv);

    CefSettings settings;
    settings.multi_threaded_message_loop = true;
    CefString(&settings.browser_subprocess_path) = subProcessHelperPath;
    CefString(&settings.log_file) = cefDebugLogPath;
    CefString(&settings.resources_dir_path) = m_data->GetWorkingDirectory();

    JVMSignals::Backup();

    bool result = CefInitialize(args, settings, m_app.get(), nullptr);

    JVMSignals::Restore();

    if (!result) {
        m_data->SetState(Base::ApplicationState::FAILED);

        return;
    }
}
