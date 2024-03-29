// SPDX-License-Identifier: BSD-2-Clause

#include "include/browsercontrol.h"

#include "Browser.hpp"

namespace {

[[nodiscard]] std::wstring convertJavaStringToWString(JNIEnv* env, jstring input)
{
    std::wstring result;

    if (input == nullptr) {
        return result;
    }

    const jchar* raw = env->GetStringChars(input, nullptr);

    if (raw == nullptr) {
        return result;
    }

    jsize length = env->GetStringLength(input);

    result.assign(raw, raw + length);

    env->ReleaseStringChars(input, raw);

    return result;
}

}

JNIEXPORT jboolean JNICALL Java_nativeadvert_browsercontrol_browsercontrol0(
    JNIEnv* env, [[maybe_unused]] jclass thisObj, jobject advertCanvas, jstring URL)
{
    std::wstring initialDestination = convertJavaStringToWString(env, URL);

    if (initialDestination.empty()) {
        return JNI_FALSE;
    }

    bool result = Browser::The().GetContext()->Initialize(env, advertCanvas, std::move(initialDestination));

    if (result) {
        return JNI_TRUE;
    }

    return JNI_FALSE;
}

JNIEXPORT void JNICALL Java_nativeadvert_browsercontrol_destroy0(
    [[maybe_unused]] JNIEnv* env, [[maybe_unused]] jclass thisObj)
{
    Browser::The().GetContext()->Destroy();
}

JNIEXPORT void JNICALL Java_nativeadvert_browsercontrol_navigate0(
    JNIEnv* env, [[maybe_unused]] jclass thisObj, jstring URL)
{
    std::wstring destination = convertJavaStringToWString(env, URL);

    if (destination.empty()) {
        return;
    }

    Browser::The().GetContext()->Navigate(std::move(destination));
}

JNIEXPORT void JNICALL Java_nativeadvert_browsercontrol_resize0(
    [[maybe_unused]] JNIEnv* env, [[maybe_unused]] jclass thisObj, jint width, jint height)
{
    Browser::The().GetContext()->Resize(width, height);
}
