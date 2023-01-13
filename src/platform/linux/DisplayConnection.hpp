// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <memory>

#include <xcb/xcb.h>

/**
 * Wraps our connection to the X Server and provides a clean interface to the
 * functionality we need.
 */
class DisplayConnection
{
public:
    static std::unique_ptr<DisplayConnection> Connect() noexcept;

    ~DisplayConnection() noexcept;

    DisplayConnection(const DisplayConnection&) = delete;
    DisplayConnection& operator=(const DisplayConnection&) = delete;

private:
    static xcb_connection_t *MakeConnection(int *preferredScreen) noexcept;
    static xcb_screen_t *FindPreferredScreen(xcb_connection_t*, int) noexcept;

    explicit DisplayConnection(xcb_connection_t*, xcb_screen_t*) noexcept;

    xcb_connection_t *m_connection;
    xcb_screen_t *m_screen;
};
