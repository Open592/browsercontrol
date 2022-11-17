# SPDX-License-Identifier: BSD-2-Clause

# Support fetching NuGet packages
#
# Added primarily to support fetching WebView2 for Windows platforms
#
# OUTPUT_NAME :: CMAKE variable name to associate with this package PACKAGE_NAME
# :: NuGet package name we want to fetch VERSION :: Version of the package we
# want to fetch
function(nugetfetch OUTPUT_NAME PACKAGE_NAME PACKAGE_VERSION)
  find_program(NUGET nuget)

  set(NUGET_CLI_EXECUTABLE "nuget-cli.exe")
  set(NUGET_CLI_PATH
      "${CMAKE_CURRENT_BINARY_DIR}/tools/${NUGET_CLI_EXECUTABLE}")
  set(NUGET_EXECUTABLE)

  if(NUGET STREQUAL "NUGET-NOTFOUND")
    if(NOT EXISTS ${NUGET_CLI_PATH})
      message(
        STATUS "[NuGet] :: Failed to find NuGet executable. Downloading...")
      file(DOWNLOAD
           "https://dist.nuget.org/win-x86-commandline/latest/nuget.exe"
           ${NUGET_CLI_PATH})
    endif()

    set(NUGET_EXECUTABLE ${NUGET_CLI_PATH})
  elseif()
    set(NUGET_EXECUTABLE ${NUGET})
  endif()

  execute_process(
    COMMAND
      ${NUGET_EXECUTABLE} install ${PACKAGE_NAME} -Version ${PACKAGE_VERSION}
      -ExcludeVersion -OutputDirectory ${CMAKE_CURRENT_BINARY_DIR}/packages
    OUTPUT_QUIET COMMAND_ERROR_IS_FATAL ANY)

  set(${OUTPUT_NAME}-PATH
      "${CMAKE_CURRENT_BINARY_DIR}/packages/${PACKAGE_NAME}"
      PARENT_SCOPE)
endfunction()
