# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 3.9)

project(browsercontrol
    VERSION 0.0.1
    DESCRIPTION "Cross platform browsercontrol library"
    HOMEPAGE_URL "https://github.com/Open592/browsercontrol"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

get_property(generatorIsMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

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
        src/platform/windows/Win32BrowserControl.cpp
        src/platform/windows/WebView2BrowserWindow.cpp
    )
endif()

configure_file(browsercontrol.pc.in browsercontrol.pc @ONLY)

target_include_directories(browsercontrol
    PRIVATE
        ${JNI_INCLUDE_DIRS}
        .
)

target_link_libraries(browsercontrol PRIVATE ${JNI_LIBRARIES})

install(TARGETS browsercontrol
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES ${CMAKE_BINARY_DIR}/browsercontrol.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
)

if (INCLUDE_EXAMPLES)
    add_subdirectory(examples)

    get_target_property(EXAMPLE_JAR_LOCATION examplejar JAR_FILE)

    message(STATUS ${EXAMPLE_JAR_LOCATION})

    if (generatorIsMultiConfig)
        add_custom_command(TARGET browsercontrol POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${EXAMPLE_JAR_LOCATION} $<TARGET_FILE_DIR:browsercontrol>
            COMMENT "Copy JAR file to resulting build directory for multi config generators"
        )
    endif()
endif()
