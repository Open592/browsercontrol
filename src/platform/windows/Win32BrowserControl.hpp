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

    virtual ~Win32BrowserControl() = default;
    virtual bool Initialize(JNIEnv*, jobject) noexcept override;
    virtual void Destroy() noexcept override;
    virtual void Resize(int32_t, int32_t) noexcept override;
    virtual void Navigate(std::string_view) noexcept override;

private:
    static HWND ResolveParentWindow(JNIEnv*, jobject);

    HWND m_browserWindow;
};

#endif /* WIN32BROWSERCONTROL_H */
