# SPDX-License-Identifier: BSD-2-Clause

# Support fetching NuGet packages
#
# Added primarily to support fetching WebView2 for Windows platforms
#
# OUTPUT_NAME :: CMAKE variable name to associate with this package
#
# PACKAGE_NAME :: NuGet package name we want to fetch
#
# VERSION :: Version of the package we want to fetch
function(nugetfetch OUTPUT_NAME PACKAGE_NAME PACKAGE_VERSION)
  find_program(NUGET_EXE nuget)

  if(NOT NUGET_EXE)
    message(FATAL_ERROR "Failed to find nuget.exe! Please install, and run cmake again")
  endif()

  execute_process(
    COMMAND ${NUGET_EXE} install ${PACKAGE_NAME} -Version ${PACKAGE_VERSION} -ExcludeVersion -OutputDirectory
            ${CMAKE_BINARY_DIR}/packages OUTPUT_QUIET COMMAND_ERROR_IS_FATAL ANY
  )

  set(${OUTPUT_NAME}-PATH
      "${CMAKE_BINARY_DIR}/packages/${PACKAGE_NAME}"
      PARENT_SCOPE
  )
endfunction()
