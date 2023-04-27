// SPDX-License-Identifier: BSD-2-Clause

#include <jawt.h>

#include "LinuxBrowserData.hpp"

/**
 * We require that our host process passes us information about the current working
 * directory of the executable. We use this information to pass the browser subprocess
 * helper path to chromium.
 *
 * This information is passed in Java system properties under the following key:
 *
 * com.open592.workingDirectory
 *
 * We require that the browser helper is a sibling of the applet viewer.
 */
bool LinuxBrowserData::ResolveWorkingDirectory(JNIEnv* env) noexcept
{
    jclass system = env->FindClass("java/lang/System");

    if (system == nullptr) {
        // Check for `ClassNotFoundException`
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        }

        return false;
    }

    jmethodID getProperty = env->GetStaticMethodID(system, "getProperty", "(Ljava/lang/String;)Ljava/lang/String;");

    if (getProperty == nullptr) {
        // Check for `NoSuchMethodError`
        if (env->ExceptionOccurred()) {
            env->ExceptionClear();
        }

        return false;
    }

    jstring propertyName = env->NewStringUTF("com.open592.workingDirectory");

    if (propertyName == nullptr) {
        // Check for `NullPointerException`
        if (env->ExceptionOccurred()) {
            env->ExceptionClear();
        }

        return false;
    }

    // reinterpret_cast is fine here since we know our call to CallStaticObjectMethod will return a jstring
    auto workingDirectory = reinterpret_cast<jstring>(env->CallStaticObjectMethod(system, getProperty, propertyName));

    if (workingDirectory == nullptr) {
        if (env->ExceptionOccurred()) {
            env->ExceptionClear();
        }

        return false;
    }

    const char* property = env->GetStringUTFChars(workingDirectory, JNI_FALSE);

    if (property == nullptr) {
        return false;
    }

    m_workingDirectory = std::filesystem::canonical(property);

    env->ReleaseStringUTFChars(workingDirectory, property);

    return true;
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
            env->ExceptionDescribe();
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
