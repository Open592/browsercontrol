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
    const std::vector<std::pair<std::string, std::string>> browserCommands = {
        std::pair("xdg-open", ""),
        std::pair("gio", " open --"),
        std::pair("x-www-browser", ""),
        std::pair("google-chrome", ""),
        std::pair("chromium-browser", ""),
        std::pair("firefox", " -new-tab"),
        std::pair("microsoft-edge", ""),
        std::pair("brave-browser", ""),
        std::pair("epiphany", ""),
        std::pair("opera", ""),
        std::pair("google-chrome-stable", ""),
        std::pair("chrome", ""),
        std::pair("chromium", ""),
    };

    for (const auto& [command, commandLineFlags] : browserCommands) {
        const std::optional<std::string> path = which(command);

        if (path.has_value()) {
            // If the browser requires additional command line commandLineFlags we include them here
            return commandLineFlags.empty() ? path.value() : path.value() + commandLineFlags;
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
