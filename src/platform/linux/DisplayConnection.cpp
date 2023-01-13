// SPDX-License-Identifier: BSD-2-Clause

#include "DisplayConnection.hpp"

// static
std::unique_ptr<DisplayConnection> DisplayConnection::Connect() noexcept
{
    int preferredScreenNum = 0;
    xcb_connection_t* connection = MakeConnection(&preferredScreenNum);

    if (connection == nullptr) {
        return nullptr;
    }

    xcb_screen_t* preferredScreen = FindPreferredScreen(connection, preferredScreenNum);

    if (preferredScreen == nullptr) {
        return nullptr;
    }

    return std::unique_ptr<DisplayConnection>(new DisplayConnection(connection, preferredScreen));
}

DisplayConnection::~DisplayConnection() noexcept { xcb_disconnect(m_connection); }

DisplayConnection::DisplayConnection(xcb_connection_t* connection, xcb_screen_t* screen) noexcept
    : m_connection(connection)
    , m_screen(screen)
{
}

xcb_screen_t* DisplayConnection::FindPreferredScreen(xcb_connection_t* connection, int preferredScreenNum) noexcept
{
    const xcb_setup_t* setup = xcb_get_setup(connection);
    xcb_screen_iterator_t itr = xcb_setup_roots_iterator(setup);

    for (int i = preferredScreenNum; itr.rem; xcb_screen_next(&itr), --i) {
        if (i == 0) {
            return itr.data;
        }
    }

    return nullptr;
}

// static
xcb_connection_t* DisplayConnection::MakeConnection(int* preferredScreen) noexcept
{
    xcb_connection_t* connection = xcb_connect(nullptr, preferredScreen);

    int error = xcb_connection_has_error(connection);

    if (error > 0) {
        xcb_disconnect(connection);

        return nullptr;
    }

    return connection;
}
