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
std::optional<std::string> which(const std::string& bin)
{
    const std::string cmd = "which " + bin;
    std::string result;
    std::array<char, 256> buffer {};
    FILE* fp;

    if ((fp = popen(cmd.c_str(), "r")) == nullptr) {
        return std::nullopt;
    }

    while (fgets(buffer.data(), buffer.size(), fp)) {
        result += buffer.data();
    }

    const auto status = pclose(fp);

    // We expect `which` to return an exit status of 0
    if (status != 0 || result.empty()) {
        return std::nullopt;
    }

    // Remove trailing `\n`
    return result.erase(result.length() - 1);
}

/**
 * Attempt to find the best executable which will allow us
 * to open the user's default browser
 *
 * @returns Path to the executable along with any command line
 * flags which are required to open a URL within the browser.
 */
std::string resolveBrowserCommand()
{
    const std::array<std::pair<std::string, std::string>, 13> browsers = {
        std::make_pair("xdg-open", ""),
        std::make_pair("gio", " open --"),
        std::make_pair("x-www-browser", ""),
        std::make_pair("google-chrome", ""),
        std::make_pair("chromium-browser", ""),
        std::make_pair("firefox", " -new-tab"),
        std::make_pair("microsoft-edge", ""),
        std::make_pair("brave-browser", ""),
        std::make_pair("epiphany", ""),
        std::make_pair("opera", ""),
        std::make_pair("google-chrome-stable", ""),
        std::make_pair("chrome", ""),
        std::make_pair("chromium", ""),
    };

    for (const auto& [command, commandLineFlags] : browsers) {
        const std::optional<std::string> path = which(command);

        if (path.has_value()) {
            // If the browser requires additional command line commandLineFlags we include them here
            return commandLineFlags.empty() ? path.value() : path.value() + commandLineFlags;
        }
    }

    return {};
}

/**
 * Store the path to the command which will allow us to open the user's browser
 */
const std::string g_desktopBrowserCommand = resolveBrowserCommand();

}

namespace DesktopBrowserLauncher {

bool Open(const std::string& URL)
{
    const std::string command = g_desktopBrowserCommand + ' ' + URL;

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
