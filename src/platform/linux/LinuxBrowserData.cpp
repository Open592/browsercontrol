// SPDX-License-Identifier: BSD-2-Clause

#include <jawt.h>
#include <optional>

#include "LinuxBrowserData.hpp"

namespace {

/**
 * We allow the host application to pass us information about their preferred
 * working directory. We then use this information to bootstrap CEF with the
 * correct location of it's required files and the browser subprocess helper.
 *
 * This information is passed in Java system properties under the following key:
 *
 * com.open592.workingDirectory
 */
std::optional<std::filesystem::path> ResolveWorkingDirectoryJavaProperty(JNIEnv* env) noexcept
{
    jclass system = env->FindClass("java/lang/System");

    if (system == nullptr) {
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        }

        return std::nullopt;
    }

    jmethodID getPropertyMethodID
        = env->GetStaticMethodID(system, "getProperty", "(Ljava/lang/String;)Ljava/lang/String;");

    if (getPropertyMethodID == nullptr) {
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        }

        return std::nullopt;
    }

    jstring propertyName = env->NewStringUTF("com.open592.workingDirectory");

    if (propertyName == nullptr) {
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        }

        return std::nullopt;
    }

    // reinterpret_cast is fine here since we know our call to CallStaticObjectMethod will return a jstring
    auto workingDirectory
        = reinterpret_cast<jstring>(env->CallStaticObjectMethod(system, getPropertyMethodID, propertyName));

    if (workingDirectory == nullptr) {
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        }

        return std::nullopt;
    }

    const char* property = env->GetStringUTFChars(workingDirectory, JNI_FALSE);

    if (property == nullptr) {
        return std::nullopt;
    }

    auto result = std::filesystem::canonical(property);

    env->ReleaseStringUTFChars(workingDirectory, property);
    env->DeleteLocalRef(propertyName);

    return result;
}

}

/**
 * Resolve the working directory of the parent application.
 *
 * We allow the parent application to pass an explicit path using
 * -Dcom.open592.workingDirectory otherwise we use the current
 * working directory.
 *
 * We look for the browser subprocess helper executable at this
 * location.
 */
void LinuxBrowserData::ResolveWorkingDirectory(JNIEnv* env) noexcept
{
    auto explicitPath = ResolveWorkingDirectoryJavaProperty(env);

    if (explicitPath.has_value()) {
        m_workingDirectory = std::move(explicitPath.value());

        return;
    }

    m_workingDirectory = std::filesystem::current_path();
}

/**
 * Resolve the host's window.
 *
 * This is the window from the Java side which we will be parenting
 * our container window.
 */
bool LinuxBrowserData::ResolveHostWindow(JNIEnv* env, jobject canvas) noexcept
{
    JAWT awt;

    // Note: In the original browsercontrol.dll this was set as JAWT_VERSION_1_4
    awt.version = JAWT_VERSION_9;

    if (JAWT_GetAWT(env, &awt) != JNI_TRUE) {
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        }

        return false;
    }

    JAWT_DrawingSurface* drawingSurface = awt.GetDrawingSurface(env, canvas);

    if (drawingSurface == nullptr) {
        return false;
    }

    jint lock = drawingSurface->Lock(drawingSurface);

    if ((lock & JAWT_LOCK_ERROR) != 0) {
        awt.FreeDrawingSurface(drawingSurface);

        return false;
    }

    JAWT_DrawingSurfaceInfo* drawingSurfaceInfo = drawingSurface->GetDrawingSurfaceInfo(drawingSurface);

    if (drawingSurfaceInfo == nullptr) {
        drawingSurface->Unlock(drawingSurface);
        awt.FreeDrawingSurface(drawingSurface);

        return false;
    }

    auto* x11DrawingSurfaceInfo = static_cast<JAWT_X11DrawingSurfaceInfo*>(drawingSurfaceInfo->platformInfo);

    if (x11DrawingSurfaceInfo == nullptr) {
        drawingSurface->FreeDrawingSurfaceInfo(drawingSurfaceInfo);
        drawingSurface->Unlock(drawingSurface);
        awt.FreeDrawingSurface(drawingSurface);

        return false;
    }

    auto result = static_cast<xcb_window_t>(x11DrawingSurfaceInfo->drawable);

    drawingSurface->FreeDrawingSurfaceInfo(drawingSurfaceInfo);
    drawingSurface->Unlock(drawingSurface);
    awt.FreeDrawingSurface(drawingSurface);

    m_hostWindow = result;

    return true;
}
