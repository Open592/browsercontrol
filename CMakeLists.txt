# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 3.9)

project(browsercontrol
        VERSION 0.0.1
        DESCRIPTION "Cross platform browsercontrol library"
        HOMEPAGE_URL "https://github.com/Open592/browsercontrol"
        LANGUAGES C CXX
        )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(
            "/permissive"
            "$<$<CONFIG:DEBUG>:/Od>"
            "$<$<CONFIG:DEBUG>:/Z7>"
            "$<$<CONFIG:DEBUG>:/DEBUG>"
            "$<$<CONFIG:RELEASE>:/Ox>"
    )
endif ()

include(GNUInstallDirs)
include(FindJNI)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(browsercontrol
        SHARED
        src/browsercontrol.cpp
        src/BrowserContext.cpp
        )

set_target_properties(browsercontrol
        PROPERTIES
        VERSION ${PROJECT_VERSION}
        PUBLIC_HEADER include/browsercontrol.h
        )

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(browsercontrol
            PRIVATE
            src/platform/windows/init.cpp
            src/platform/windows/BrowserData.cpp
            src/platform/windows/Win32BrowserControl.cpp
            src/platform/windows/WebView2BrowserWindow.cpp
            )
endif ()

configure_file(browsercontrol.pc.in browsercontrol.pc @ONLY)

target_include_directories(browsercontrol
        PRIVATE
        ${JNI_INCLUDE_DIRS}
        .
        )

target_link_libraries(browsercontrol PRIVATE ${JNI_LIBRARIES})

# For Windows we need to include the WebView2 SDK
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    include(NuGetFetch)

    NuGetFetch(WebView2 "Microsoft.Web.WebView2" "1.0.1222-prerelease")

    target_include_directories(browsercontrol PRIVATE ${WebView2-PATH}/build/native/include)

    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        target_link_libraries(browsercontrol PRIVATE ${WebView2-PATH}/build/native/x64/WebView2LoaderStatic.lib)
    else ()
        target_link_libraries(browsercontrol PRIVATE ${WebView2-PATH}/build/native/x86/WebView2LoaderStatic.lib)
    endif ()
endif ()

install(TARGETS browsercontrol
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

install(FILES ${CMAKE_BINARY_DIR}/browsercontrol.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
        )

if (INCLUDE_EXAMPLES)
    add_subdirectory(examples)
endif ()
