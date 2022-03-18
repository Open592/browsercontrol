// SPDX-License-Identifier: BSD-2-Clause

#include <jawt.h>

#include "include/browsercontrol.h"

#include "BrowserContext.hpp"

jboolean Java_nativeadvert_browsercontrol_browsercontrol0(
    JNIEnv* env, jclass thisObj, jobject advertCanvas, jstring URL)
{
    const char* initialDestination = env->GetStringUTFChars(URL, nullptr);

    if (initialDestination == nullptr) {
        return JNI_FALSE;
    }

    bool result = BrowserContext::the().InitializeBrowserWindow(env, advertCanvas, initialDestination);

    env->ReleaseStringUTFChars(URL, initialDestination);

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
    const char* destination = env->GetStringUTFChars(URL, nullptr);

    BrowserContext::the().Navigate(destination);

    env->ReleaseStringUTFChars(URL, destination);
}

void Java_nativeadvert_browsercontrol_resize0(JNIEnv* env, jclass thisObj, jint width, jint height)
{
    BrowserContext::the().ResizeBrowserWindow(width, height);
}
