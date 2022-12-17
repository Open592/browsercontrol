# SPDX-License-Identifier: BSD-2-Clause

cmake_policy(SET CMP0079 NEW)

# cmake-format: off
target_compile_options(browsercontrol PRIVATE
        "-Wall"
        "-Wextra"
        "-Wshadow"
        "-Wnon-virtual-dtor"
        "-pedantic"
        "-Wold-style-cast"
        "-Wcast-align"
        "-Wunused"
        "$<$<CONFIG:DEBUG>:-g>"
        "$<$<CONFIG:DEBUG>:-pg>"
        "$<$<CONFIG:DEBUG>:-O0>"
        "$<$<CONFIG:RELEASE>:-O2>"
)
# cmake-format: on

target_precompile_headers(browsercontrol PRIVATE pch.hpp)

find_package(X11 REQUIRED)
find_package(CEF REQUIRED)

target_link_libraries(browsercontrol PRIVATE ${X11_LIBRARIES} ${CEF_LIBRARY} ${CEFWRAPPER_LIBRARY})
target_include_directories(browsercontrol PRIVATE ${X11_INCLUDE_DIR} ${CEF_INCLUDE_DIR})

target_sources(browsercontrol PRIVATE CEFBrowserControl.cpp init.cpp)
