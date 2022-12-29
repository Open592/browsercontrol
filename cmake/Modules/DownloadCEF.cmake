# SPDX-License-Identifier: BSD-2-Clause

#
# A Linux specific utility to download a specific version of CEF (Chromium Embedded Framework) from their official CDN.
#
# Once downloaded we will utilize the packaged FindCEF.cmake to locate and link the CEF library and associated
# dependencies.
#
# Copyright (c) 2016 The Chromium Embedded Framework Authors.
#
# Credit to cef-project (https://bitbucket.org/chromiumembedded/cef-project) for their original DownloadCEF.cmake from
# which this file was heavily inspired.
#

# This function will handle architecture resolution internally.
function(download_cef version download_dir)
  # We only utilize CEF for Linux machines.
  if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(FATAL_ERROR "Attempted to utilize DownloadCEF on a non-Linux machine!")
  endif()

  # Validate that we are running a supported architecture
  if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "x86_64")
    set(CEF_PLATFORM "linux64")
  elseif(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "aarch64")
    set(CEF_PLATFORM "linuxarm64")
  elseif(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "armv7l")
    set(CEF_PLATFORM "linuxarm")
  else()
    message(FATAL_ERROR "CEF is required, but not supported on your architecture")
  endif()

  # Name of the distribution of CEF we will be downloading and extracting
  set(CEF_DISTRIBUTION "cef_binary_${version}_${CEF_PLATFORM}_minimal")
  # Directory where the archive and SHA1 files will be downloaded.
  set(CEF_DOWNLOAD_DIR "${download_dir}")
  # Path of the extracted files
  set(CEF_ROOT
      "${CEF_DOWNLOAD_DIR}/${CEF_DISTRIBUTION}"
      CACHE INTERNAL "CEF_ROOT"
  )

  if(NOT IS_DIRECTORY "${CEF_ROOT}")
    set(CEF_DOWNLOAD_FILENAME "${CEF_DISTRIBUTION}.tar.bz2")
    # Path of the archive file
    set(CEF_DOWNLOAD_PATH "${CEF_DOWNLOAD_DIR}/${CEF_DOWNLOAD_FILENAME}")

    # No need to download the archive again if we already have the file
    if(NOT EXISTS "${CEF_DOWNLOAD_PATH}")
      set(CEF_DOWNLOAD_URL "https://cef-builds.spotifycdn.com/${CEF_DOWNLOAD_FILENAME}")
      string(REPLACE "+" "%2B" CEF_DOWNLOAD_URL_ESCAPED ${CEF_DOWNLOAD_URL})

      message(STATUS "=====!!!NOTE:!!!=====")
      message(STATUS "Downloading CEF from: ${CEF_DOWNLOAD_URL_ESCAPED}")
      message(STATUS "This will significantly increase configure time!")
      message(STATUS "You may use `CEF_ROOT` to specify a local path for CEF to avoid this.")
      message(STATUS "=====!!!NOTE!!!=====")

      message(STATUS "Downloading SHA1 for ${CEF_DOWNLOAD_URL_ESCAPED}...")
      file(DOWNLOAD "${CEF_DOWNLOAD_URL_ESCAPED}.sha1" "${CEF_DOWNLOAD_PATH}.sha1")
      file(READ "${CEF_DOWNLOAD_PATH}.sha1" CEF_SHA1)

      file(
        DOWNLOAD "${CEF_DOWNLOAD_URL_ESCAPED}" "${CEF_DOWNLOAD_PATH}"
        EXPECTED_HASH SHA1=${CEF_SHA1}
        SHOW_PROGRESS
      )
    endif()

    # Extract the archive to ${CEF_ROOT}
    message(STATUS "Extracting ${CEF_DOWNLOAD_PATH}...")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xvf "${CEF_DOWNLOAD_PATH}" WORKING_DIRECTORY ${CEF_DOWNLOAD_DIR})
  endif()
endfunction()
