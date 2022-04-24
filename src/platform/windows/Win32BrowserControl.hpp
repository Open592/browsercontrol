// SPDX-License-Identifier: BSD-2-Clause

#ifndef WIN32BROWSERCONTROL_HPP
#define WIN32BROWSERCONTROL_HPP

#include "Windows.h"
#include <jawt_md.h>
#include <jni.h>
#include <memory>

#include "../../AbstractBrowserControl.hpp"
#include "BrowserData.hpp"

class Win32BrowserControl : public AbstractBrowserControl {
public:
    explicit Win32BrowserControl(std::shared_ptr<BrowserData>&&);

    ~Win32BrowserControl() override;
    bool Initialize(JNIEnv*, jobject, const char*) noexcept override;
    void Destroy() noexcept override;
    void Resize(int32_t, int32_t) noexcept override;
    void Navigate(const char*) noexcept override;

private:
    [[nodiscard]] static HWND ResolveParentWindow(JNIEnv*, jobject);
    static DWORD WINAPI ThreadProc(LPVOID);

    DWORD StartMessagePump();

    // Manual reset events
    HANDLE m_browserWindowCreateEvent = nullptr;

    HWND m_browserWindow = nullptr;
    HWND m_parentWindow = nullptr;

    std::shared_ptr<BrowserData> m_browserData;
};

#endif // WIN32BROWSERCONTROL_HPP
