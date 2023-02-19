// SPDX-License-Identifier: BSD-2-Clause

#include "JVMSignals.hpp"

#include "LinuxBrowserControl.hpp"

LinuxBrowserControl::LinuxBrowserControl() noexcept
    : m_data(std::make_shared<BrowserData>())
{
}

LinuxBrowserControl::~LinuxBrowserControl() noexcept = default;

bool LinuxBrowserControl::IsRunning() const noexcept { return m_data->IsRunning(); }

bool LinuxBrowserControl::Initialize(JNIEnv* env, jobject canvas, std::wstring initialDestination) noexcept
{
    CefWindowHandle host = ResolveHostWindow(env, canvas);

    if (host == kNullWindowHandle) {
        return false;
    }

    std::filesystem::path workingDirectory = ResolveWorkingDirectory(env);

    if (workingDirectory.empty()) {
        return false;
    }

    auto browserSubProcessHelperPath = workingDirectory / "browsercontrol_helper";
    auto debugLogFilePath = workingDirectory / "cef_debug.log";

    m_data->SetDestination(initialDestination);

    char* argv[] = { (char*)"browsercontrol" };
    CefMainArgs args(1, argv);

    CefSettings settings;
    CefString(&settings.browser_subprocess_path) = browserSubProcessHelperPath.string();
    settings.multi_threaded_message_loop = true;
    CefString(&settings.log_file) = debugLogFilePath.string();
    CefString(&settings.resources_dir_path) = workingDirectory.string();

    m_app = new BrowserControlApp(m_data, host);

    // CefInitialize will overwrite JVM signal handlers. This causes issues with JIT'd code
    // and results in random crashes. Backup the present signal handlers, and after
    // returning from CefInitialize restore them to their previous actions.
    //
    // See: https://bitbucket.org/chromiumembedded/java-cef/issues/41/mac-jcef-frequently-crashing-in-thread
    JVMSignals::Backup();

    bool res = CefInitialize(args, settings, m_app.get(), nullptr);

    if (!res) {
        m_data->SetState(BrowserData::State::FAILED_TO_START);

        return false;
    }

    JVMSignals::Restore();

    return m_data->WaitForInitializationResult();
}

void LinuxBrowserControl::Destroy() noexcept { return; }

void LinuxBrowserControl::Resize(int32_t, int32_t) noexcept { return; }

void LinuxBrowserControl::Navigate(std::wstring) noexcept { return; }

// static
CefWindowHandle LinuxBrowserControl::ResolveHostWindow(JNIEnv* env, jobject canvas) noexcept
{
    JAWT awt;

    // In the original browsercontrol.dll this was set as JAWT_VERSION_1_4
    awt.version = JAWT_VERSION_9;

    if (JAWT_GetAWT(env, &awt) != JNI_TRUE) {
        return kNullWindowHandle;
    }

    JAWT_DrawingSurface* drawingSurface = awt.GetDrawingSurface(env, canvas);

    if (drawingSurface == nullptr) {
        return kNullWindowHandle;
    }

    jint lock = drawingSurface->Lock(drawingSurface);

    if ((lock & JAWT_LOCK_ERROR) != 0) {
        awt.FreeDrawingSurface(drawingSurface);

        return kNullWindowHandle;
    }

    JAWT_DrawingSurfaceInfo* drawingSurfaceInfo = drawingSurface->GetDrawingSurfaceInfo(drawingSurface);

    if (drawingSurfaceInfo == nullptr) {
        awt.FreeDrawingSurface(drawingSurface);

        return kNullWindowHandle;
    }

    auto* x11DrawingSurfaceInfo = static_cast<JAWT_X11DrawingSurfaceInfo*>(drawingSurfaceInfo->platformInfo);

    if (x11DrawingSurfaceInfo == nullptr) {
        drawingSurface->FreeDrawingSurfaceInfo(drawingSurfaceInfo);
        drawingSurface->Unlock(drawingSurface);
        awt.FreeDrawingSurface(drawingSurface);

        return kNullWindowHandle;
    }

    auto result = static_cast<CefWindowHandle>(x11DrawingSurfaceInfo->drawable);

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
