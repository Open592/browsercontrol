# SPDX-License-Identifier: BSD-2-Clause

cmake_policy(SET CMP0079 NEW)

include(DownloadCEF)
download_cef("108.4.13+ga98cd4c+chromium-108.0.5359.125" "${CMAKE_CURRENT_SOURCE_DIR}/third_party/cef")

list(APPEND CMAKE_MODULE_PATH ${CEF_ROOT}/cmake)

find_package(X11 REQUIRED)
find_package(CEF REQUIRED)

add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)

# Create libcef lib target
add_library(libcef_lib SHARED IMPORTED)
set_target_properties(libcef_lib PROPERTIES IMPORTED_LOCATION ${CEF_LIB_RELEASE})

add_library(browsercontrol SHARED ${BROWSERCONTROL_SHARED_FILES} init.cpp CEFBrowserControl.cpp)

set_library_target_properties(browsercontrol)

set_target_properties(
  browsercontrol
  PROPERTIES VERSION ${PROJECT_VERSION}
             ARCHIVE_OUTPUT_DIRECTORY ${BROWSERCONTROL_TARGET_OUT_DIR}
             RUNTIME_OUTPUT_DIRECTORY ${BROWSERCONTROL_TARGET_OUT_DIR}
             LIBRARY_OUTPUT_DIRECTORY ${BROWSERCONTROL_TARGET_OUT_DIR}
)

add_dependencies(browsercontrol libcef_dll_wrapper)
target_precompile_headers(browsercontrol PUBLIC pch.hpp)
target_include_directories(browsercontrol PRIVATE ${JNI_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR})
target_link_libraries(
  browsercontrol
  PUBLIC libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS}
  PRIVATE ${JNI_LIBRARIES}
)

copy_files(browsercontrol "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR_RELEASE}" "${BROWSERCONTROL_TARGET_OUT_DIR}")
copy_files(browsercontrol "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${BROWSERCONTROL_TARGET_OUT_DIR}")
