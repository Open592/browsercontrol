# SPDX-License-Identifier: BSD-2-Clause

cmake_policy(SET CMP0079 NEW)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # cmake-format: off
  add_compile_options(
    "/permissive"
    "$<$<CONFIG:DEBUG>:/Od>"
    "$<$<CONFIG:DEBUG>:/Z7>"
    "$<$<CONFIG:DEBUG>:/DEBUG>"
    "$<$<CONFIG:RELEASE>:/O2>"
  )
  # cmake-format: on
else()
  message(FATAL_ERROR "MSVC is the only compiler currently supported on Windows!")
endif()

include(nugetfetch)

nugetfetch(WebView2 "Microsoft.Web.WebView2" "1.0.1418.22")

if(NOT EXISTS ${WebView2-PATH})
  message(FATAL_ERROR "Failed to retrieve WebView2 - Unable to proceed!")
endif()

nugetfetch(WindowsImplementationLibrary Microsoft.Windows.ImplementationLibrary "1.0.220201.1")

if(NOT EXISTS ${WindowsImplementationLibrary-PATH})
  message(FATAL_ERROR "Failed to retrieve Windows Implementation Library - Unable to proceed!")
endif()

# Let MSBuild know we want to statically link WebView2
set_target_properties(browsercontrol PROPERTIES VS_GLOBAL_WebView2LoaderPreference Static)

target_link_libraries(browsercontrol PRIVATE ${WebView2-PATH}/build/native/Microsoft.Web.WebView2.targets)
target_link_libraries(
  browsercontrol
  PRIVATE ${WindowsImplementationLibrary-PATH}/build/native/Microsoft.Windows.ImplementationLibrary.targets
)

target_sources(browsercontrol PRIVATE init.cpp BrowserData.cpp Win32BrowserControl.cpp WebView2BrowserWindow.cpp)

target_precompile_headers(browsercontrol PRIVATE pch.hpp)

# Required for URLDownloadToFile
target_link_libraries(browsercontrol PRIVATE urlmon.lib)
