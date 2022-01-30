// SPDX-License-Identifier: BSD-2-Clause

#include <jawt.h>
#include <memory>

#include "include/browsercontrol.h"

#include "BrowserContext.hpp"

static std::shared_ptr<BrowserContext> g_browserContext;

jboolean Java_nativeadvert_browsercontrol_browsercontrol0(JNIEnv* env, jclass thisObj, jobject advertCanvas, jstring url)
{
    return JNI_TRUE;
}

void Java_nativeadvert_browsercontrol_destroy0(JNIEnv* env, jclass thisObj)
{
}

void Java_nativeadvert_browsercontrol_navigate0(JNIEnv* env, jclass thisObj, jstring URL)
{
    std::string_view toURL = env->GetStringUTFChars(URL, nullptr);

    g_browserContext->navigate(toURL);
}

void Java_nativeadvert_browsercontrol_resize0(JNIEnv* env, jclass thisObj, jint width, jint height)
{
    g_browserContext->resize(width, height);
}
