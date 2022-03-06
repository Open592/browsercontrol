// SPDX-License-Identifier: BSD-2-Clause

#ifndef WIN32BROWSERCONTROL_H
#define WIN32BROWSERCONTROL_H

#include "windows.h"
#include <jawt_md.h>
#include <jni.h>
#include <memory>
#include <string_view>

#include "../../AbstractBrowserControl.hpp"

class Win32BrowserControl : public AbstractBrowserControl {
public:
    Win32BrowserControl();

    ~Win32BrowserControl() override = default;
    bool Initialize(JNIEnv*, jobject) noexcept override;
    void Destroy() noexcept override;
    void Resize(int32_t, int32_t) noexcept override;
    void Navigate(std::string_view) noexcept override;

private:
    static HWND ResolveParentWindow(JNIEnv*, jobject);

    HWND m_browserWindow = nullptr;
};

#endif /* WIN32BROWSERCONTROL_H */
