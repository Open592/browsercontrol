// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <xcb/xcb.h>

#include "BrowserEventLoop.hpp"
#include "LinuxBrowserData.hpp"

class DisplayConnection {
public:
    DisplayConnection() = default;

    ~DisplayConnection() noexcept
    {
        if (m_connection) {
            xcb_disconnect(m_connection);
        }
    }

    bool MakeConnection() noexcept
    {
        int preferredScreen = 0;
        m_connection = Connect(&preferredScreen);

        if (!m_connection) {
            return false;
        }

        m_screen = FindPreferredScreen(m_connection, preferredScreen);

        if (!m_screen) {
            return false;
        }

        return true;
    }

private:
    static xcb_connection_t* Connect(int* screen)
    {
        xcb_connection_t* connection = xcb_connect(nullptr, screen);

        int connection_error = xcb_connection_has_error(connection);

        if (connection_error) {
            // Still need to free connection
            xcb_disconnect(connection);

            return nullptr;
        }

        return connection;
    }

    /**
     * Find the preferred screen, as identified by `xcb_connect`.
     *
     * When calling `xcb_connect` we are passed a preferred screen number. By default this is `0`
     * but in the case this is something else we will iterate over the available screens
     * and return back the one which was requested.
     */
    static xcb_screen_t* FindPreferredScreen(xcb_connection_t* connection, int preferredScreen)
    {
        int currentScreen = 0;

        for (xcb_screen_iterator_t it = xcb_setup_roots_iterator(xcb_get_setup(connection)); it.rem;
             xcb_screen_next(&it)) {
            if (currentScreen == preferredScreen) {
                return it.data;
            }

            ++currentScreen;
        }

        return nullptr;
    }

    xcb_connection_t* m_connection;
    xcb_screen_t* m_screen;
};

class BrowserWindow {
public:
    explicit BrowserWindow(LinuxBrowserData&, BrowserEventLoop&) noexcept;

    bool CreateHostWindow();

private:
    DisplayConnection m_displayConnection;
    LinuxBrowserData& m_data;
    BrowserEventLoop& m_eventLoop;
};
