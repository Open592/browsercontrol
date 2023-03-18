// SPDX-License-Identifier: BSD-2-Clause

#include <functional>
#include <iostream>
#include <sys/select.h>

#include <include/base/cef_bind.h>

#include "LinuxBrowserControl.hpp"

namespace {

constexpr auto PARENT_WINDOW_NAME = "jbw";

}

LinuxBrowserControl::LinuxBrowserControl() noexcept
    : m_data(std::make_shared<BrowserData>())
    , m_windowMessageLoop {}
{
    if (!MakeDisplayConnection()) {
        // Failed to make connection to X server
        m_data->SetState(BrowserData::State::TERMINATED);

        return;
    }
}

LinuxBrowserControl::~LinuxBrowserControl() noexcept
{
    m_windowMessageLoop.join();

    xcb_disconnect(m_connection);
}

bool LinuxBrowserControl::IsRunning() const noexcept { return m_data->IsRunning(); }

bool LinuxBrowserControl::Initialize(JNIEnv* env, jobject canvas, std::wstring initialDestination) noexcept
{
    if (m_data->GetState() == BrowserData::State::TERMINATED) {
        return false;
    }

    xcb_window_t hostWindow = ResolveHostWindow(env, canvas);

    if (hostWindow == XCB_NONE) {
        m_data->SetState(BrowserData::State::TERMINATED);

        return false;
    }

    std::filesystem::path workingDirectory = ResolveWorkingDirectory(env);

    if (workingDirectory.empty()) {
        m_data->SetState(BrowserData::State::TERMINATED);

        return false;
    }

    m_data->SetDestination(initialDestination);

    m_parentWindow = CreateParentWindow(hostWindow);

    m_browserProcess = std::make_unique<CefBrowserProcess>(m_data, m_parentWindow, workingDirectory);
    m_windowMessageLoop = std::thread([&] {
        if (!StartWindowMessageHandler()) {
            m_data->SetState(BrowserData::State::TERMINATED);
        }
    });

    return m_data->WaitForInitializationResult();
}

void LinuxBrowserControl::Destroy() noexcept
{
    m_browserProcess->DispatchMessage(CefBrowserProcess::Destroy);

    m_data->WaitForStateTransition(BrowserData::State::RUNNING);
}

void LinuxBrowserControl::Resize(int32_t, int32_t) noexcept { }

void LinuxBrowserControl::Navigate(std::wstring) noexcept { }

// static
xcb_window_t LinuxBrowserControl::ResolveHostWindow(JNIEnv* env, jobject canvas) noexcept
{
    JAWT awt;

    // In the original browsercontrol.dll this was set as JAWT_VERSION_1_4
    awt.version = JAWT_VERSION_9;

    if (JAWT_GetAWT(env, &awt) != JNI_TRUE) {
        return XCB_NONE;
    }

    JAWT_DrawingSurface* drawingSurface = awt.GetDrawingSurface(env, canvas);

    if (drawingSurface == nullptr) {
        return XCB_NONE;
    }

    jint lock = drawingSurface->Lock(drawingSurface);

    if ((lock & JAWT_LOCK_ERROR) != 0) {
        awt.FreeDrawingSurface(drawingSurface);

        return XCB_NONE;
    }

    JAWT_DrawingSurfaceInfo* drawingSurfaceInfo = drawingSurface->GetDrawingSurfaceInfo(drawingSurface);

    if (drawingSurfaceInfo == nullptr) {
        awt.FreeDrawingSurface(drawingSurface);

        return XCB_NONE;
    }

    auto* x11DrawingSurfaceInfo = static_cast<JAWT_X11DrawingSurfaceInfo*>(drawingSurfaceInfo->platformInfo);

    if (x11DrawingSurfaceInfo == nullptr) {
        drawingSurface->FreeDrawingSurfaceInfo(drawingSurfaceInfo);
        drawingSurface->Unlock(drawingSurface);
        awt.FreeDrawingSurface(drawingSurface);

        return XCB_NONE;
    }

    auto result = static_cast<xcb_window_t>(x11DrawingSurfaceInfo->drawable);

    drawingSurface->FreeDrawingSurfaceInfo(drawingSurfaceInfo);
    drawingSurface->Unlock(drawingSurface);
    awt.FreeDrawingSurface(drawingSurface);

    return result;
}

// static
std::filesystem::path LinuxBrowserControl::ResolveWorkingDirectory(JNIEnv* env) noexcept
{
    jclass system = env->FindClass("java/lang/System");

    if (system == nullptr || env->ExceptionCheck() == JNI_TRUE) {
        return {};
    }

    jmethodID getProperty = env->GetStaticMethodID(system, "getProperty", "(Ljava/lang/String;)Ljava/lang/String;");

    if (getProperty == nullptr || env->ExceptionCheck() == JNI_TRUE) {
        return {};
    }

    jstring propertyName = env->NewStringUTF("com.open592.workingDirectory");

    if (propertyName == nullptr || env->ExceptionCheck() == JNI_TRUE) {
        return {};
    }

    auto workingDirectory = (jstring)(env->CallStaticObjectMethod(system, getProperty, propertyName));

    if (workingDirectory == nullptr || env->ExceptionCheck() == JNI_TRUE) {
        return {};
    }

    const char* property = env->GetStringUTFChars(workingDirectory, JNI_FALSE);

    if (property == nullptr) {
        return {};
    }

    std::filesystem::path result = std::filesystem::canonical(property);

    env->ReleaseStringUTFChars(workingDirectory, property);

    return result;
}

xcb_window_t LinuxBrowserControl::CreateParentWindow(xcb_window_t hostWindow)
{
    xcb_window_t window = xcb_generate_id(m_connection);

    const uint32_t events[] = { XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY };
    xcb_create_window(m_connection, XCB_COPY_FROM_PARENT, window, hostWindow, 0, 0, 1024, 96, 0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT, m_screen->root_visual, XCB_CW_EVENT_MASK, events);

    xcb_change_property(m_connection, XCB_PROP_MODE_REPLACE, window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
        std::strlen(PARENT_WINDOW_NAME), PARENT_WINDOW_NAME);

    xcb_map_window(m_connection, window);

    xcb_flush(m_connection);

    return window;
}

bool LinuxBrowserControl::MakeDisplayConnection()
{
    int screenNum = 0;

    xcb_connection_t* connection = xcb_connect(nullptr, &screenNum);

    int connectionError = xcb_connection_has_error(connection);

    if (connectionError > 0) {
        // Still need to free connection
        xcb_disconnect(connection);

        return false;
    }

    // Find the default screen
    const xcb_setup_t* setup = xcb_get_setup(connection);
    xcb_screen_iterator_t itr = xcb_setup_roots_iterator(setup);
    xcb_screen_t* preferredScreen = nullptr;

    for (int i = screenNum; itr.rem; xcb_screen_next(&itr), --i) {
        if (i == 0) {
            preferredScreen = itr.data;
        }
    }

    if (preferredScreen == nullptr) {
        xcb_disconnect(connection);

        return false;
    }

    // Set values
    m_connection = connection;
    m_screen = preferredScreen;

    return true;
}

bool LinuxBrowserControl::StartWindowMessageHandler()
{
    xcb_generic_event_t* event = XCB_NONE;
    fd_set fds;
    int fd = xcb_get_file_descriptor(m_connection);

    // If there has not been an X event in 25 milliseconds then we time out
    // and check for any errors
    const auto ms = std::chrono::milliseconds(10000);
    struct timespec timeout = { .tv_sec = 0, .tv_nsec = static_cast<long>(ms.count() * 1000) };

    while (m_data->GetState() != BrowserData::State::TERMINATED) {
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        if (xcb_connection_has_error(m_connection)) {
            // We received an error while our application is currently running.
            // This is an issue.
            goto HandleError;
        }

        int eventCount = pselect(fd + 1, &fds, nullptr, nullptr, &timeout, nullptr);

        if (eventCount == 0) {
            // Nothing to do
            continue;
        }

        if (eventCount < 0) {
            // Failed to fetch events from fd
            goto HandleError;
        }

        while ((event = xcb_poll_for_event(m_connection)) != nullptr) {
            m_browserProcess->DispatchWindowMessage(*event);

            free(event);

            xcb_flush(m_connection);
        }
    }

    return true;

HandleError:
    m_data->SetState(BrowserData::State::TERMINATED);

    return false;
}
