# SPDX-License-Identifier: BSD-2-Clause

include(FetchContent)
include(FindPackageHandleStandardArgs)

# FindCEF
#
# This is a cmake module which will ensure CEF (Chromium Embedded Framework) is installed on the machine.
#
# You can read more about CEF here: https://bitbucket.org/chromiumembedded/cef/src/master/
#
# Thank you to the OBS Project for their implementation of FindCEF which I used as a reference:
# https://github.com/obsproject/obs-browser/blob/master/FindCEF.cmake
#
# Logic of FindCEF
#
# If `LOCAL_CEF_ROOT_DIR` is defined attempt to find the CEF distribution at that location
#
# If `LOCAL_CEF_ROOT_DIR` is *NOT* set, *OR* it doesn't point to a valid CEF distribution we will attempt to download a
# valid distribution for the machine arch.

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
  message(FATAL_ERROR "Attempted to utilize FindCEF on a non-Linux machine!")
endif()

# We will only be including CEF for our Linux builds
#
# * x86_64
# * ARM
# * ARM64
set(CEF_VERSION "cef_binary_107.1.12+g65b79a6+chromium-107.0.5304.122")

# Validate that we are running a supported architecture
if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "x86_64")
  set(CEF_MACHINE_ARCH "linux64")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "aarch64")
  set(CEF_MACHINE_ARCH "linuxarm64")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "armv7l")
  set(CEF_MACHINE_ARCH "linuxarm")
else()
  message(FATAL_ERROR "CEF is required, but not supported on your architecture")
endif()

# Attempt to find CEF on the local FS
if(LOCAL_CEF_ROOT_DIR)
  message(STATUS "Attempting to find CEF distribution within ${LOCAL_CEF_ROOT_DIR}")
  message(STATUS "NOTE: We are expecting a version compatible with ${CEF_VERSION}!")

  find_path(CEF_INCLUDE_DIR "include/cef_version.h" HINTS "${LOCAL_CEF_ROOT_DIR}")

  find_library(
    CEF_LIBRARY
    NAMES libcef.so "Chromium Embedded Framework"
    NO_DEFAULT_PATH
    PATHS "${LOCAL_CEF_ROOT_DIR}" "${LOCAL_CEF_ROOT_DIR}/Release"
  )

  find_library(
    CEFWRAPPER_LIBRARY
    NAMES libcef_dll_wrapper.a
    NO_DEFAULT_PATH
    PATHS "${LOCAL_CEF_ROOT_DIR}" "${LOCAL_CEF_ROOT_DIR}/Release" "${LOCAL_CEF_ROOT_DIR}/Release/libcef_dll_wrapper"
          "${LOCAL_CEF_ROOT_DIR}/build/libcef_dll_wrapper"
  )

  if(CEF_INCLUDE_DIR
     AND CEF_LIBRARY
     AND CEFWRAPPER_LIBRARY
  )
    set(LOCATED_LOCAL_CEF TRUE)
  elseif()
    set(LOCATED_LOCAL_CEF FALSE)

    unset(CEF_INCLUDE_DIR)
    unset(CEF_LIBRARY)
    unset(CEFWRAPPER_LIBRARY)

    message(WARNING "Failed to find valid CEF distribution within ${LOCAL_CEF_ROOT_DIR}")
  endif()
endif()

# If we haven't found a distribution locally we need to fetch from the CDN
if(NOT LOCATED_LOCAL_CEF)
  set(CEF_NAME "${CEF_VERSION}_${CEF_MACHINE_ARCH}")
  set(CEF_URL "https://github.com/Open592/cef-release/releases/download/${CEF_VERSION}/${CEF_NAME}.tar.bz2")

  message(STATUS "=====!!!NOTE:!!!=====")
  message(STATUS "Downloading CEF from: ${CEF_URL}")
  message(STATUS "This will significantly increase configure time!")
  message(STATUS "You may use `LOCAL_CEF_ROOT_DIR` to specify a local path for CEF to avoid this.")
  message(STATUS "=====!!!NOTE!!!=====")

  set(FETCHCONTENT_BASE_DIR "${CMAKE_BINARY_DIR}/packages/external/cef")
  set(FETCHCONTENT_QUIET OFF)

  FetchContent_Declare(cef URL ${CEF_URL})
  FetchContent_MakeAvailable(cef)

  find_path(CEF_INCLUDE_DIR "include/cef_version.h" HINTS "${cef_SOURCE_DIR}")

  find_library(
    CEF_LIBRARY
    NAMES libcef.so "Chromium Embedded Framework"
    NO_DEFAULT_PATH
    PATHS "${cef_SOURCE_DIR}"
  )

  find_library(
    CEFWRAPPER_LIBRARY
    NAMES libcef_dll_wrapper.a
    NO_DEFAULT_PATH
    PATHS "${cef_SOURCE_DIR}"
  )
endif()

if(NOT CEF_INCLUDE_DIR)
  message(WARNING "Failed to find CEF_INCLUDE_DIR")
  set(CEF_FOUND FALSE)
  return()
elseif(NOT CEF_LIBRARY)
  message(WARNING "Failed to find CEF_LIBRARY")
  set(CEF_FOUND FALSE)
  return()
elseif(NOT CEFWRAPPER_LIBRARY)
  message(WARNING "Failed to find CEFWRAPPER_LIBRARY")
  set(CEF_FOUND FALSE)
  return()
elseif()
  message(STATUS "Found CEF: ${CEF_LIBRARY};${CEFWRAPPER_LIBRARY}")
endif()

find_package_handle_standard_args(CEF DEFAULT_MSG CEF_INCLUDE_DIR CEF_LIBRARY CEFWRAPPER_LIBRARY)

mark_as_advanced(CEF_INCLUDE_DIR CEF_LIBRARY CEFWRAPPER_LIBRARY)
