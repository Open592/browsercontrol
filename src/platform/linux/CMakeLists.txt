# SPDX-License-Identifier: BSD-2-Clause

include(DownloadCEF)

download_cef("111.2.6+g491d238+chromium-111.0.5563.65" "${CMAKE_SOURCE_DIR}/third_party/cef")

list(APPEND CMAKE_MODULE_PATH ${CEF_ROOT}/cmake)

find_package(X11 REQUIRED)
find_package(CEF REQUIRED)

print_cef_config()

# cmake-format: off
add_compile_options(
        "-pedantic"
        "-Wall"
        "$<$<CONFIG:DEBUG>:-g>"
        "$<$<CONFIG:DEBUG>:-pg>"
        "$<$<CONFIG:DEBUG>:-Werror>"
        "$<$<CONFIG:RELEASE>:-O3>"
)
# cmake-format: on

add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)

add_logical_target(libcef_lib ${CEF_LIB_DEBUG} ${CEF_LIB_RELEASE})

set(CEF_TARGET_OUT_DIR ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR})

# Library target
add_library(
  browsercontrol SHARED
  ${BROWSERCONTROL_SHARED_SOURCES}
  init.cpp
  BrowserApp.cpp
  BrowserEventLoop.cpp
  JVMSignals.cpp
  LinuxBrowserContext.cpp
  LinuxBrowserData.cpp
)
target_precompile_headers(browsercontrol PRIVATE pch.hpp)
set_library_target_properties(browsercontrol)
add_dependencies(browsercontrol libcef_dll_wrapper)
target_include_directories(browsercontrol PUBLIC ${CMAKE_SOURCE_DIR} ${JNI_INCLUDE_DIRS})
target_link_libraries(browsercontrol libcef_lib libcef_dll_wrapper ${JNI_LIBRARIES} ${CEF_STANDARD_LIBS})
set_browsercontrol_target_properties(browsercontrol)

# Helper executable target
add_executable(browsercontrol_helper helper.cpp)
set_executable_target_properties(browsercontrol_helper)
add_dependencies(browsercontrol_helper libcef_dll_wrapper)
target_link_libraries(browsercontrol_helper libcef_lib libcef_dll_wrapper)
set_browsercontrol_target_properties(browsercontrol_helper)

# TODO: We should select only the required components needed for our limited use-case See: README.txt within CEF
# distribution
copy_files(browsercontrol "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
copy_files(browsercontrol "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")

set_linux_suid_permissions(browsercontrol "${CEF_TARGET_OUT_DIR}/chrome-sandbox")
