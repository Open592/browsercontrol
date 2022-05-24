// SPDX-License-Identifier: BSD-2-Clause

#ifndef WIN32BROWSERCONTROL_HPP
#define WIN32BROWSERCONTROL_HPP

#include "../../AbstractBrowserControl.hpp"
#include "BrowserData.hpp"

class Win32BrowserControl : public AbstractBrowserControl {
public:
    explicit Win32BrowserControl(std::shared_ptr<BrowserData>&&);

    ~Win32BrowserControl() override;
    bool Initialize(JNIEnv*, jobject, const char*) noexcept override;
    bool IsRunning() const noexcept override;
    void Destroy() noexcept override;
    void Resize(int32_t, int32_t) noexcept override;
    void Navigate(const char*) noexcept override;

private:
    enum class BrowserWindowStatus : uint8_t {
        NOT_STARTED,
        RUNNING,
        FAILED_TO_START,
        TERMINATED,
    };

    [[nodiscard]] static HWND ResolveHostWindow(JNIEnv*, jobject);

    void StartMessagePump();

    HWND m_browserWindow = nullptr;
    HWND m_hostWindow = nullptr;

    std::shared_ptr<BrowserData> m_browserData;
    std::atomic<BrowserWindowStatus> m_browserWindowStatus;
    [[maybe_unused]] std::jthread m_browserWindowThread;
};

#endif // WIN32BROWSERCONTROL_HPP
