// SPDX-License-Identifier: BSD-2-Clause

#include "include/browsercontrol.h"

#include "BrowserContext.hpp"

jboolean Java_nativeadvert_browsercontrol_browsercontrol0(
    JNIEnv* env, jclass thisObj, jobject advertCanvas, jstring URL)
{
    const jchar* initialDestination = env->GetStringChars(URL, nullptr);

    if (initialDestination == nullptr) {
        return JNI_FALSE;
    }

    bool result = BrowserContext::the().InitializeBrowserWindow(env, advertCanvas, initialDestination);

    env->ReleaseStringChars(URL, initialDestination);

    if (result) {
        return JNI_TRUE;
    }

    return JNI_FALSE;
}

void Java_nativeadvert_browsercontrol_destroy0(JNIEnv* env, jclass thisObj)
{
    BrowserContext::the().DestroyBrowserWindow();
}

void Java_nativeadvert_browsercontrol_navigate0(JNIEnv* env, jclass thisObj, jstring URL)
{
    const jchar* destination = env->GetStringChars(URL, nullptr);

    BrowserContext::the().Navigate(destination);

    env->ReleaseStringChars(URL, destination);
}

void Java_nativeadvert_browsercontrol_resize0(JNIEnv* env, jclass thisObj, jint width, jint height)
{
    BrowserContext::the().ResizeBrowserWindow(width, height);
}
