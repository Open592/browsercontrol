// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#define WIN32_LEAN_AND_MEAN
#define UNICODE
#include <Windows.h>
#include <processthreadsapi.h> // For CreateProcess
#include <shellapi.h> // For ShellExecute
#include <urlmon.h> // For URLDownloadToFile

#include <ShlObj.h>
#include <wil/com.h>
#include <wrl.h>

#include "WebView2.h"

#include <jawt_md.h>
#include <jni.h>

#include <condition_variable>
#include <filesystem>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <utility>
