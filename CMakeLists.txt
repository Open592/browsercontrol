# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 3.13.0)

project(
  browsercontrol
  VERSION 0.1.0
  DESCRIPTION "Cross platform browsercontrol library"
  HOMEPAGE_URL "https://github.com/Open592/browsercontrol"
  LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_TEMPLATES_DIRECTORY ${PROJECT_SOURCE_DIR}/cmake/templates)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

include(GNUInstallDirs)
include(FindJNI)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(browsercontrol SHARED src/browsercontrol.cpp src/BrowserContext.cpp src/BrowserData.cpp)

set_target_properties(browsercontrol PROPERTIES VERSION ${PROJECT_VERSION} PUBLIC_HEADER include/browsercontrol.h)

configure_file(${CMAKE_TEMPLATES_DIRECTORY}/META.in ${CMAKE_SOURCE_DIR}/META @ONLY)

target_include_directories(browsercontrol PRIVATE ${JNI_INCLUDE_DIRS} .)

target_link_libraries(browsercontrol PRIVATE ${JNI_LIBRARIES})

install(
  TARGETS browsercontrol
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES ${CMAKE_BINARY_DIR}/browsercontrol.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  add_subdirectory(src/platform/windows)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_subdirectory(src/platform/linux)
else()
  message(FATAL_ERROR "This platform is not currently supported!")
endif()

if(INCLUDE_EXAMPLES)
  add_subdirectory(examples)
endif()
