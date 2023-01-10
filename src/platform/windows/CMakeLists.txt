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

include(NugetFetch)

nuget_fetch(WebView2 "Microsoft.Web.WebView2" "1.0.1418.22")

if(NOT EXISTS ${WebView2-PATH})
  message(FATAL_ERROR "Failed to retrieve WebView2 - Unable to proceed!")
endif()

nuget_fetch(WindowsImplementationLibrary Microsoft.Windows.ImplementationLibrary "1.0.220201.1")

if(NOT EXISTS ${WindowsImplementationLibrary-PATH})
  message(FATAL_ERROR "Failed to retrieve Windows Implementation Library - Unable to proceed!")
endif()

add_library(
  browsercontrol SHARED ${BROWSERCONTROL_SHARED_SOURCES} init.cpp Win32BrowserControl.cpp WebView2BrowserWindow.cpp
)

target_precompile_headers(browsercontrol PRIVATE pch.hpp)

# Let MSBuild know we want to statically link WebView2
set_target_properties(browsercontrol PROPERTIES VS_GLOBAL_WebView2LoaderPreference Static)

# urlmon.lib is required for URLDownloadToFile
target_link_libraries(
  browsercontrol PRIVATE ${JNI_LIBRARIES} urlmon.lib ${WebView2-PATH}/build/native/Microsoft.Web.WebView2.targets
)

# UGLY: From CMAKE 3.8 passing a `.targets` file to `target_link_libraries` should properly import the referenced
# targets into the resulting project files. This works for the build, but unfortunately every IDE I've tried does not
# properly handle this for intellisense.
#
# Because of this we are both utilizing `target_link_libraries` with the `.targets` file, and
# `target_include_directories` for the WebView2 target
target_include_directories(
  browsercontrol PRIVATE ${JNI_INCLUDE_DIRS} ${WebView2-PATH}/build/native/include
                         ${WindowsImplementationLibrary-PATH}/include ${CMAKE_SOURCE_DIR}
)

set_browsercontrol_target_properties(browsercontrol)
