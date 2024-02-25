// SPDX-License-Identifier: BSD-2-Clause

#include <array>
#include <cstdio>
#include <optional>

#include "DesktopBrowserLauncher.hpp"

/**
 * This file is heavily inspired from the "webbrowser" CPython module.
 *
 * https://github.com/python/cpython/blob/main/Lib/webbrowser.py
 */

namespace {

/**
 * Represents a command which opens the user's desktop web browser, along with
 * any optional command line arguments which will be passed along to the binary.
 */
struct WebBrowserCommand {
    // Used at runtime to locate the binary on the user's system.
    std::string binaryName;
    // If present, these arguments are appended to the resolved path of the binary.
    std::optional<std::string> commandLineArguments;
};

/**
 * Helper function to execute `which`
 *
 * We use this to test for the presence of potential
 * executables which could open the user's browser
 */
std::optional<std::string> which(const std::string& programName)
{
    const std::string command = "which " + programName;
    FILE* pipe = popen(command.c_str(), "r");

    if (pipe == nullptr) {
        return std::nullopt;
    }

    // Read result from pipe
    std::array<char, 1024> chunk {};
    std::string result;

    while (fgets(chunk.data(), chunk.size(), pipe) != nullptr) {
        result += chunk.data();
    }

    const auto status = pclose(pipe);

    // We expect `which` to return an exit status of 0
    if (status != 0 || result.empty()) {
        return std::nullopt;
    }

    if (result.back() == '\n') {
        return result.erase(result.length() - 1);
    } else {
        return result;
    }
}

/**
 * Attempt to find the best executable which will allow us
 * to open the user's default browser
 *
 * @returns Path to the executable along with any command line
 * flags which are required to open a URL within the browser.
 */
std::optional<std::string> resolveBrowserCommand()
{
    const std::vector<WebBrowserCommand> browserCommands = {
        { "xdg-open", std::nullopt },
        { "gio", " open --" },
        { "x-www-browser", std::nullopt },
        { "google-chrome", std::nullopt },
        { "chromium-browser", std::nullopt },
        { "firefox", " -new-tab" },
        { "microsoft-edge", std::nullopt },
        { "brave-browser", std::nullopt },
        { "epiphany", std::nullopt },
        { "opera", std::nullopt },
        { "google-chrome-stable", std::nullopt },
        { "chrome", std::nullopt },
        { "chromium", std::nullopt },
    };

    for (const auto& [binaryName, commandLineArguments] : browserCommands) {
        const std::optional<std::string> path = which(binaryName);

        if (path.has_value()) {
            return commandLineArguments.has_value() ? path.value() + commandLineArguments.value() : path.value();
        }
    }

    return std::nullopt;
}

/**
 * Store the path to the command which will allow us to open the user's browser
 */
const std::optional<std::string> g_desktopBrowserCommand = resolveBrowserCommand();

}

namespace DesktopBrowserLauncher {

bool Open(const std::string& URL)
{
    if (!g_desktopBrowserCommand.has_value()) {
        return false;
    }

    const std::string command = g_desktopBrowserCommand.value() + ' ' + URL;
    int status = std::system(command.c_str());

    if (status < 0) {
        return false;
    }

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status) == 0;
    }

    return false;
}

}
