// SPDX-License-Identifier: BSD-2-Clause

#include <jawt.h>
#include <memory>

#include "include/browsercontrol.h"

#include "BrowserContext.hpp"

jboolean Java_nativeadvert_browsercontrol_browsercontrol0(
    JNIEnv* env, jclass thisObj, jobject advertCanvas, jstring url)
{
    if (BrowserContext::the().InitializeBrowserWindow(env, advertCanvas)) {
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
    std::string_view toURL = env->GetStringUTFChars(URL, nullptr);

    BrowserContext::the().NavigateToURL(toURL);
}

void Java_nativeadvert_browsercontrol_resize0(JNIEnv* env, jclass thisObj, jint width, jint height)
{
    BrowserContext::the().ResizeBrowserWindow(width, height);
}
