set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)
include(FindJNI)

set(BROWSERCONTROL_SHARED_SOURCES ${SOURCES_ROOT}/browsercontrol.cpp ${SOURCES_ROOT}/Browser.cpp
                                  ${SOURCES_ROOT}/BrowserContext.cpp ${SOURCES_ROOT}/BrowserData.cpp
)

macro(SET_BROWSERCONTROL_TARGET_PROPERTIES target)
  set_target_properties(
    ${target}
    PROPERTIES VERSION ${PROJECT_VERSION}
               INSTALL_RPATH "$ORIGIN"
               BUILD_WITH_INSTALL_RPATH TRUE
               ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}
               RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
               LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}
  )
endmacro()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  add_subdirectory(platform/windows)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_subdirectory(platform/linux)
else()
  message(FATAL_ERROR "This platform is not currently supported!")
endif()
