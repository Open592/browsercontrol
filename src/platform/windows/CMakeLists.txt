# SPDX-License-Identifier: BSD-2-Clause

cmake_policy(SET CMP0079 NEW)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_compile_options(
    "/permissive" "$<$<CONFIG:DEBUG>:/Od>" "$<$<CONFIG:DEBUG>:/Z7>"
    "$<$<CONFIG:DEBUG>:/DEBUG>" "$<$<CONFIG:RELEASE>:/O2>")
else()
  message(
    FATAL_ERROR "MSVC is the only compiler currently supported on Windows!")
endif()

target_sources(
  browsercontrol PRIVATE init.cpp BrowserData.cpp Win32BrowserControl.cpp
                         WebView2BrowserWindow.cpp)

target_precompile_headers(browsercontrol PRIVATE pch.hpp)

# Required for URLDownloadToFile
target_link_libraries(browsercontrol PRIVATE urlmon.lib)

include(nugetfetch)

nugetfetch(WebView2 "Microsoft.Web.WebView2" "1.0.1418.22")

if(NOT EXISTS ${WebView2-PATH})
  message(FATAL_ERROR "Failed to retrieve WebView2 - Unable to proceed!")
endif()

nugetfetch(WindowsImplementationLibrary Microsoft.Windows.ImplementationLibrary
           "1.0.220201.1")

if(NOT EXISTS ${WindowsImplementationLibrary-PATH})
  message(
    FATAL_ERROR
      "Failed to retrieve Windows Implementation Library - Unable to proceed!")
endif()

target_include_directories(browsercontrol
                           PRIVATE ${WebView2-PATH}/build/native/include)
target_include_directories(browsercontrol
                           PRIVATE ${WindowsImplementationLibrary-PATH}/include)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "ARM64")
  set(WEBVIEW2_ARCH_DIRECTORY ${WebView2-PATH}/build/native/arm64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(WEBVIEW2_ARCH_DIRECTORY ${WebView2-PATH}/build/native/x64)
else()
  set(WEBVIEW2_ARCH_DIRECTORY ${WebView2-PATH}/build/native/x86)
endif()

target_link_libraries(
  browsercontrol PRIVATE ${WEBVIEW2_ARCH_DIRECTORY}/WebView2LoaderStatic.lib)
