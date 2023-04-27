// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <xcb/xcb.h>

class ContainerWindow {
public:
    class Delegate { };

    explicit ContainerWindow(const Delegate&) noexcept;

    ~ContainerWindow();

    /**
     * Creates the container window, and initializes the event listeners
     *
     * @return True if successful
     */
    bool Create(xcb_window_t);

    /**
     * Return the X window handle of the container window
     *
     * @return The X window handle
     */
    xcb_window_t GetWindowHandle() const noexcept;

    /**
     * Initializes the connection to the X server
     *
     * @return True if successful
     */
    bool MakeXConnection() noexcept;

    void StartEventLoop();

    void StopEventLoop();

private:
    static xcb_connection_t* Connect(int* screen);

    /**
     * Find the preferred screen, as identified by `xcb_connect`.
     *
     * When calling `xcb_connect` we are passed a preferred screen number. By default this is `0`
     * but in the case this is something else we will iterate over the available screens
     * and return back the one which was requested.
     */
    static xcb_screen_t* FindPreferredScreen(xcb_connection_t* connection, int preferredScreen);

    /**
     * Event loop worker
     */
    void EventLoop();

    /**
     * Process events from the container window
     */
    bool ProcessXEvent(const xcb_generic_event_t&) const;

    enum class EventLoopStatus {
        STOPPED,
        RUNNING,
    };

    std::unique_ptr<std::thread> m_eventLoopThread;

    const Delegate& m_delegate;

    xcb_connection_t* m_connection;
    xcb_screen_t* m_screen;

    EventLoopStatus m_status = EventLoopStatus::STOPPED;

    // Container window handle
    xcb_window_t m_window;

    // Stores the WM_DELETE_WINDOW client message event
    xcb_intern_atom_reply_t* m_wmDeleteWindowAtom;
};
