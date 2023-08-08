// SPDX-License-Identifier: BSD-2-Clause

/**
 * This separate executable will be invoked automatically by CEF
 * when spawning sub-processes (For example the render process)
 *
 * The host application will provide a path to this executable
 * which we will receive and pass along to CEF through the
 * `browser_subprocess_path` setting.
 *
 * We need to use this method since we don't have a single
 * entrypoint executable which could be automatically re-invoked
 * by CEF.
 *
 * See more within the CEF docs:
 * https://bitbucket.org/chromiumembedded/cef/wiki/GeneralUsage#markdown-header-entry-point-function
 */

#include <include/cef_app.h>
#include <include/cef_command_line.h>

int main(int argc, char** argv)
{
    CefSettings settings;

#if defined(DEBUG)
    settings.log_severity = LOGSEVERITY_ERROR;
#elif defined(RELEASE)
    settings.log_severity = LOGSEVERITY_DISABLE;
#endif

    CefMainArgs mainArgs(argc, argv);

    return CefExecuteProcess(mainArgs, nullptr, nullptr);
}
