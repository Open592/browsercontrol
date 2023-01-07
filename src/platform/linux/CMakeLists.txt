# SPDX-License-Identifier: BSD-2-Clause

include(DownloadCEF)

download_cef("108.4.13+ga98cd4c+chromium-108.0.5359.125" "${CMAKE_SOURCE_DIR}/third_party/cef")

list(APPEND CMAKE_MODULE_PATH ${CEF_ROOT}/cmake)

find_package(X11 REQUIRED)
find_package(CEF REQUIRED)

print_cef_config()

add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)

set(CEF_TARGET browsercontrol)

add_logical_target(libcef_lib ${CEF_LIB_DEBUG} ${CEF_LIB_RELEASE})

set(CEF_TARGET_OUT_DIR "${CMAKE_BINARY_DIR}/artifacts")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR})

add_executable(
  ${CEF_TARGET} simple_app.cc simple_app.h simple_handler.cc simple_handler.h cefsimple_linux.cc
                simple_handler_linux.cc
)

set_executable_target_properties(${CEF_TARGET})

add_dependencies(${CEF_TARGET} libcef_dll_wrapper)

target_include_directories(${CEF_TARGET} PUBLIC ${JNI_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR})
target_link_libraries(${CEF_TARGET} libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS})

set_target_properties(
  ${CEF_TARGET}
  PROPERTIES VERSION ${PROJECT_VERSION}
             INSTALL_RPATH "$ORIGIN"
             BUILD_WITH_INSTALL_RPATH TRUE
             ARCHIVE_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR}
             RUNTIME_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR}
             LIBRARY_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR}
)

copy_files(${CEF_TARGET} "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
copy_files(${CEF_TARGET} "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")

set_linux_suid_permissions(${CEF_TARGET} "${CEF_TARGET_OUT_DIR}/chrome-sandbox")
