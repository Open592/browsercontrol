// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <memory>

#include <jni.h>
#include <xcb/xcb.h>

#include "DisplayConnection.hpp"
#include "src/AbstractBrowserControl.hpp"
#include "src/BrowserData.hpp"

class CEFBrowserControl : public AbstractBrowserControl {
public:
    CEFBrowserControl() noexcept;

    ~CEFBrowserControl() override;
    bool Initialize(JNIEnv*, jobject, const jchar*) noexcept override;
    [[nodiscard]] bool IsRunning() const noexcept override;
    void Destroy() noexcept override;
    void Resize(int32_t, int32_t) noexcept override;
    void Navigate(const jchar*) noexcept override;

private:
    std::shared_ptr<BrowserData> m_data;
    std::unique_ptr<DisplayConnection> m_displayConnection;
};
