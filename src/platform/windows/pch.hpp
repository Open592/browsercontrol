// SPDX-License-Identifier: BSD-2-Clause

#ifndef BROWSERCONTROL_PCH_HPP
#define BROWSERCONTROL_PCH_HPP

#define WIN32_LEAN_AND_MEAN
#define UNICODE
#include <Windows.h>
#include <urlmon.h> // For URLDownloadToFile
#include <processthreadsapi.h> // For CreateProcess

#include <ShlObj.h>
#include <wil/com.h>
#include <wrl.h>

#include "WebView2.h"

#include <jawt_md.h>
#include <jni.h>

#include <atomic>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <utility>

#endif // BROWSERCONTROL_PCH_HPP
