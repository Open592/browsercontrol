# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 3.13.0)

set(CMAKE_CONFIGURATION_TYPES Debug Release)

project(
  browsercontrol
  VERSION 1.0.2
  DESCRIPTION "Cross platform browsercontrol library"
  HOMEPAGE_URL "https://github.com/Open592/browsercontrol"
  LANGUAGES C CXX
)

set(CMAKE_TEMPLATES_DIRECTORY ${PROJECT_SOURCE_DIR}/cmake/templates)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

set(SOURCES_ROOT "${CMAKE_SOURCE_DIR}/src")
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/output")

configure_file(${CMAKE_TEMPLATES_DIRECTORY}/META.in ${CMAKE_SOURCE_DIR}/META @ONLY)

add_subdirectory(${SOURCES_ROOT})

if(INCLUDE_EXAMPLES)
  add_subdirectory(examples)
endif()
