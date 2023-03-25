// SPDX-License-Identifier: BSD-2-Clause

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
std::filesystem::path LinuxBrowserData::ResolveWorkingDirectory(JNIEnv* env) noexcept
{
    jclass system = env->FindClass("java/lang/System");

    if (system == nullptr) {
        // Check for `ClassNotFoundException`
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        }

        return {};
    }

    jmethodID getProperty = env->GetStaticMethodID(system, "getProperty", "(Ljava/lang/String;)Ljava/lang/String;");

    if (getProperty == nullptr) {
        // Check for `NoSuchMethodError`
        if (env->ExceptionOccurred()) {
            env->ExceptionClear();
        }

        return {};
    }

    jstring propertyName = env->NewStringUTF("com.open592.workingDirectory");

    if (propertyName == nullptr) {
        // Check for `NullPointerException`
        if (env->ExceptionOccurred()) {
            env->ExceptionClear();
        }

        return {};
    }

    // reinterpret_cast is fine here since we know our call to CallStaticObjectMethod will return a jstring
    auto workingDirectory = reinterpret_cast<jstring>(env->CallStaticObjectMethod(system, getProperty, propertyName));

    if (workingDirectory == nullptr) {
        if (env->ExceptionOccurred()) {
            env->ExceptionClear();
        }

        return {};
    }

    const char* property = env->GetStringUTFChars(workingDirectory, JNI_FALSE);

    if (property == nullptr) {
        return {};
    }

    std::filesystem::path path = std::filesystem::canonical(property);

    env->ReleaseStringUTFChars(workingDirectory, property);

    return path;
}
