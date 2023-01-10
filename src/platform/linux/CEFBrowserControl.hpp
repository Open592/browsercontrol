// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <jni.h>

#include "src/AbstractBrowserControl.hpp"

class CEFBrowserControl : public AbstractBrowserControl {
public:
    CEFBrowserControl();

    ~CEFBrowserControl() override;
    bool Initialize(JNIEnv*, jobject, const jchar*) noexcept override;
    [[nodiscard]] bool IsRunning() const noexcept override;
    void Destroy() noexcept override;
    void Resize(int32_t, int32_t) noexcept override;
    void Navigate(const jchar *) noexcept override;

private:
};
