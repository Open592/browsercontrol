// SPDX-License-Identifier: BSD-2-Clause

#include <include/base/cef_callback.h>
#include <include/cef_base.h>

#include "src/BrowserContext.hpp"

#include "BrowserApp.hpp"
#include "JVMSignals.hpp"
#include "LinuxBrowserContext.hpp"

LinuxBrowserContext::LinuxBrowserContext(std::unique_ptr<LinuxBrowserData> data) noexcept
    : m_app(new BrowserApp(*this))
    , m_data(std::move(data))
    , m_eventLoop(std::make_unique<BrowserEventLoop>())
{
    assert(m_data && "Expected browser data to exist!");
    assert(m_eventLoop && "Expected browser event loop to exist!");

    m_browserWindow = std::make_unique<BrowserWindow>(*m_data, *m_eventLoop, *this);
}

LinuxBrowserData& LinuxBrowserContext::GetBrowserData() const noexcept { return *m_data; }

void LinuxBrowserContext::OnContextInitialized() const { m_browserWindow->Create(); }

void LinuxBrowserContext::OnBrowserWindowDestroyed()
{
    if (!m_eventLoop->CurrentlyOnBrowserThread()) {
        return m_eventLoop->EnqueueWork(
            base::BindOnce(&LinuxBrowserContext::OnBrowserWindowDestroyed, base::Unretained(this)));
    }

    m_browserWindow.reset(nullptr);

    CefShutdown();

    m_data->SetState(ApplicationState::PENDING);
}

bool LinuxBrowserContext::PerformInitialize(JNIEnv* env, jobject canvas)
{
    m_data->ResolveWorkingDirectory(env);

    if (!m_data->ResolveHostWindow(env, canvas)) {
        return false;
    }

    m_eventLoop->EnqueueWork(base::BindOnce(&LinuxBrowserContext::StartCEF, base::Unretained(this)));

    m_data->WaitForStateOrFailure(ApplicationState::STARTED);

    return m_data->IsRunning();
}

void LinuxBrowserContext::PerformDestroy()
{
    m_browserWindow->Destroy();

    m_data->WaitForStateOrFailure(ApplicationState::PENDING);
}

void LinuxBrowserContext::PerformResize() { m_browserWindow->Resize(); }

void LinuxBrowserContext::PerformNavigate() { m_browserWindow->Navigate(); }

void LinuxBrowserContext::StartCEF() const
{
    DCHECK(m_eventLoop->CurrentlyOnBrowserThread());

    const auto subProcessHelperPath = m_data->GetWorkingDirectory() / "browsercontrol_helper";

    char* argv[] = { const_cast<char*>("browsercontrol") };
    CefMainArgs args(1, argv);

    CefSettings settings;
    settings.multi_threaded_message_loop = true;
    CefString(&settings.browser_subprocess_path) = subProcessHelperPath;
    CefString(&settings.resources_dir_path) = m_data->GetWorkingDirectory();
    settings.background_color = CefColorSetARGB(0, 0, 0, 0);

#if defined(DEBUG)
    const auto cefDebugLogPath = m_data->GetWorkingDirectory() / "cef_debug.log";
    CefString(&settings.log_file) = cefDebugLogPath;
    settings.log_severity = LOGSEVERITY_ERROR;
#elif defined(RELEASE)
    settings.log_severity = LOGSEVERITY_DISABLE;
#endif

    JVMSignals::Backup();

    bool result = CefInitialize(args, settings, m_app.get(), nullptr);

    JVMSignals::Restore();

    if (!result) {
        m_data->SetState(ApplicationState::FAILED);
    }
}
