// SPDX-License-Identifier: BSD-2-Clause

#ifndef WIN32BROWSERCONTROL_H
#define WIN32BROWSERCONTROL_H

#include "Windows.h"
#include <jawt_md.h>
#include <jni.h>
#include <memory>

#include "../../AbstractBrowserControl.hpp"

class Win32BrowserControl : public AbstractBrowserControl {
public:
    Win32BrowserControl();

    ~Win32BrowserControl() override = default;
    bool Initialize(JNIEnv*, jobject, const char*) noexcept override;
    void Destroy() noexcept override;
    void Resize(int32_t, int32_t) noexcept override;
    void Navigate(const char*) noexcept override;

private:
    static HWND ResolveParentWindow(JNIEnv*, jobject);
    static DWORD WINAPI ThreadProc(LPVOID);

    DWORD StartMessagePump();

    HWND m_browserWindow = nullptr;
    HWND m_parentWindow = nullptr;
    HANDLE m_browserWindowCreateEvent = nullptr;
    HANDLE m_browserWindowThread = nullptr;
};

#endif /* WIN32BROWSERCONTROL_H */
