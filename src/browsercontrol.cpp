// SPDX-License-Identifier: BSD-2-Clause

#include "include/browsercontrol.h"

#include "BrowserContext.hpp"

[[nodiscard]] static std::wstring ConvertJavaStringToWString(JNIEnv* env, jstring input)
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

JNIEXPORT jboolean JNICALL Java_nativeadvert_browsercontrol_browsercontrol0(
    JNIEnv* env, [[maybe_unused]] jclass thisObj, jobject advertCanvas, jstring URL)
{
    std::wstring initialDestination = ConvertJavaStringToWString(env, URL);

    if (initialDestination.empty()) {
        return JNI_FALSE;
    }

    bool result = BrowserContext::the().InitializeBrowserWindow(env, advertCanvas, initialDestination);

    if (result) {
        return JNI_TRUE;
    }

    return JNI_FALSE;
}

JNIEXPORT void JNICALL Java_nativeadvert_browsercontrol_destroy0(
    [[maybe_unused]] JNIEnv* env, [[maybe_unused]] jclass thisObj)
{
    BrowserContext::the().DestroyBrowserWindow();
}

JNIEXPORT void JNICALL Java_nativeadvert_browsercontrol_navigate0(
    JNIEnv* env, [[maybe_unused]] jclass thisObj, jstring URL)
{
    std::wstring destination = ConvertJavaStringToWString(env, URL);

    BrowserContext::the().Navigate(destination);
}

JNIEXPORT void JNICALL Java_nativeadvert_browsercontrol_resize0(
    [[maybe_unused]] JNIEnv* env, [[maybe_unused]] jclass thisObj, jint width, jint height)
{
    BrowserContext::the().ResizeBrowserWindow(width, height);
}
