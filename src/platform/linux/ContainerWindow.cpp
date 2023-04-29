// SPDX-License-Identifier: BSD-2-Clause

#include <ctime>
#include <iostream>
#include <sys/select.h>

#include "ContainerWindow.hpp"

namespace {

constexpr auto HOST_WINDOW_NAME = "jbw";

}

ContainerWindow::ContainerWindow(const ContainerWindow::Delegate& delegate) noexcept
    : m_delegate(delegate)
    , m_connection(nullptr)
    , m_screen(nullptr)
    , m_window(XCB_NONE)
    , m_wmDeleteWindowAtom(XCB_NONE)
{
}

ContainerWindow::~ContainerWindow()
{
    if (m_connection != nullptr) {
        xcb_destroy_window(m_connection, m_window);

        xcb_flush(m_connection);
    }

    if (m_status == EventLoopStatus::RUNNING) {
        m_status = EventLoopStatus::STOPPED;
    }

    if (m_eventLoopThread && m_eventLoopThread->joinable()) {
        m_eventLoopThread->join();
    }

    if (m_wmDeleteWindowAtom != XCB_NONE) {
        free(m_wmDeleteWindowAtom);
    }

    xcb_disconnect(m_connection);
}

bool ContainerWindow::Create(xcb_window_t hostWindow)
{
    m_window = xcb_generate_id(m_connection);

    uint16_t mask = XCB_CW_EVENT_MASK;
    const uint32_t values[] = { XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY };

    xcb_create_window(m_connection, XCB_COPY_FROM_PARENT, m_window, hostWindow, 0, 0, 1014, 96, 0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT, m_screen->root_visual, mask, values);

    xcb_change_property(m_connection, XCB_PROP_MODE_REPLACE, m_window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
        std::strlen(HOST_WINDOW_NAME), HOST_WINDOW_NAME);

    auto wmProtocolsCookie = xcb_intern_atom(m_connection, 1, 12, "WM_PROTOCOLS");
    auto* wmProtocolsReply = xcb_intern_atom_reply(m_connection, wmProtocolsCookie, nullptr);

    if (wmProtocolsReply == XCB_NONE) {
        return false;
    }

    auto wmDeleteWindowCookie = xcb_intern_atom(m_connection, 1, 12, "WM_DELETE_WINDOW");

    // Store delete atom for later reference
    m_wmDeleteWindowAtom = xcb_intern_atom_reply(m_connection, wmDeleteWindowCookie, nullptr);

    if (m_wmDeleteWindowAtom == XCB_NONE) {
        free(wmProtocolsReply);

        return false;
    }

    xcb_change_property(
        m_connection, XCB_PROP_MODE_REPLACE, m_window, wmProtocolsReply->atom, 4, 32, 1, &m_wmDeleteWindowAtom->atom);

    xcb_map_window(m_connection, m_window);

    xcb_flush(m_connection);

    m_delegate.OnContainerWindowCreate(m_window);

    return true;
}

xcb_window_t ContainerWindow::GetWindowHandle() const noexcept { return m_window; }

bool ContainerWindow::MakeXConnection() noexcept
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

    StartEventLoop();

    return true;
}

void ContainerWindow::StartEventLoop()
{
    if (m_status == EventLoopStatus::RUNNING || m_eventLoopThread != nullptr) {
        return;
    }

    m_status = EventLoopStatus::RUNNING;

    m_eventLoopThread = std::make_unique<std::thread>(&ContainerWindow::EventLoop, this);
}

void ContainerWindow::StopEventLoop() { m_status = EventLoopStatus::STOPPED; }

// static
xcb_connection_t* ContainerWindow::Connect(int* screen)
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

// static
xcb_screen_t* ContainerWindow::FindPreferredScreen(xcb_connection_t* connection, int preferredScreen)
{
    const xcb_setup_t* setup = xcb_get_setup(connection);
    xcb_screen_iterator_t itr = xcb_setup_roots_iterator(setup);

    for (int i = 0; itr.rem; ++i, xcb_screen_next(&itr)) {
        if (i == preferredScreen) {
            return itr.data;
        }
    }

    return nullptr;
}

void ContainerWindow::EventLoop()
{
    fd_set fds;
    int xcb_fd = xcb_get_file_descriptor(m_connection);

    // If there hasn't been an X event in 25 milliseconds then we time out
    // and check for interrupts or errors
    struct timespec ts = { .tv_sec = 0, .tv_nsec = 25000000 };

    xcb_generic_event_t* event = XCB_NONE;
    while (m_status == EventLoopStatus::RUNNING) {
        FD_ZERO(&fds);
        FD_SET(xcb_fd, &fds);

        if (xcb_connection_has_error(m_connection) && m_status == EventLoopStatus::RUNNING) {
            // TODO: handle error
            return;
        }

        int eventCount = pselect(xcb_fd + 1, &fds, nullptr, nullptr, &ts, nullptr);

        if (eventCount < 0) {
            // pselect failed

            // TODO: Handle error
            return;
        }

        if (eventCount == 0) {
            // Nothing to process
            continue;
        }

        while ((event = xcb_poll_for_event(m_connection))) {
            if (!ProcessXEvent(*event)) {
                free(event);
                xcb_flush(m_connection);

                return;
            }

            free(event);
            xcb_flush(m_connection);
        }
    }
}

bool ContainerWindow::ProcessXEvent(const xcb_generic_event_t& event) const
{
    if (event.response_type == 0) {
        // Handle errors from unchecked functions
        auto error = reinterpret_cast<const xcb_generic_error_t&>(event);

        std::cout << error.error_code << '\n';
        return false;
    }

    auto code = event.response_type & ~0x80;

    switch (code) {
    case XCB_CLIENT_MESSAGE:
        std::cout << "Client message" << '\n';
        return true;
    case XCB_DESTROY_NOTIFY:
        std::cout << "Destroy notify" << '\n';
        return true;
    }

    return true;
}
