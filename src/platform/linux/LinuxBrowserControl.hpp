// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <memory>

#include <include/cef_base.h>
#include <jawt.h>
#include <jni.h>
#include <xcb/xcb.h>

#include "DisplayConnection.hpp"
#include "src/AbstractBrowserControl.hpp"
#include "src/BrowserData.hpp"

class CEFBrowserControl : public AbstractBrowserControl {
public:
    CEFBrowserControl() noexcept;

    ~CEFBrowserControl() override;
    [[nodiscard]] bool Initialize(JNIEnv*, jobject, std::wstring) noexcept override;
    [[nodiscard]] bool IsRunning() const noexcept override;
    void Destroy() noexcept override;
    void Resize(int32_t, int32_t) noexcept override;
    void Navigate(std::wstring) noexcept override;

private:
    [[nodiscard]] static CefWindowHandle ResolveHostWindow(JNIEnv*, jobject) noexcept;

    std::shared_ptr<DisplayConnection> m_display;
    std::shared_ptr<BrowserData> m_data;
};
