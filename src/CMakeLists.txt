set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)
include(FindJNI)

set(BROWSERCONTROL_SHARED_FILES ${SOURCES_ROOT}/browsercontrol.cpp ${SOURCES_ROOT}/BrowserContext.cpp
                                ${SOURCES_ROOT}/BrowserData.cpp
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  add_subdirectory(platform/windows)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_subdirectory(platform/linux)
else()
  message(FATAL_ERROR "This platform is not currently supported!")
endif()
