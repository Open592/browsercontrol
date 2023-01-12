# SPDX-License-Identifier: BSD-2-Clause

include(FindPackageHandleStandardArgs)
include(FeatureSummary)
find_package(PkgConfig)

# FindXCB
#
# This is a cmake module which, when given a list of XCB components, will ensure that they are locatable on the user's
# machine, and make them available to the caller.
#
# We require these for the Linux platform
#
# Thank you to KDE for their implementation of FindXCB which I used as a reference:
# https://github.com/KDE/extra-cmake-modules/blob/master/find-modules/FindXCB.cmake

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
  message(FATAL_ERROR "Attempted to utilize FindXCB on a non-linux platform!")
endif()

# List of required components
#
# Should be ordered so that each component follows it's dependencies
set(XCB_REQUIRED_COMPONENTS xcb)

set(XCB_FOUND true)
foreach(COMPONENT ${XCB_REQUIRED_COMPONENTS})
  # Component name for output variables (xcb -> XCB)
  string(TOUPPER ${COMPONENT} COMPONENT_NAME)
  # (XCB-EWMH -> XCB_EWMH)
  string(REPLACE "-" "_" COMPONENT_NAME ${COMPONENT_NAME})

  # Header name to search for (xcb/xcb.h -> xcb/xcb.h) (xcb/xcb-ewmh.h -> xcb/ewmh.h)
  string(REPLACE "xcb-" "" COMPONENT_HEADER xcb/${COMPONENT}.h)

  pkg_check_modules(PC_${COMPONENT} QUIET ${COMPONENT})

  find_path(
    ${COMPONENT_NAME}_INCLUDE_DIR
    NAMES ${COMPONENT_HEADER}
    HINTS ${PC_${COMPONENT}_INCLUDEDIR} ${PC_${COMPONENT}_INCLUDE_DIRS} REQUIRED
  )

  find_library(
    ${COMPONENT_NAME}_LIBRARY
    NAMES ${COMPONENT}
    HINTS ${PC_${COMPONENT}_LIBDIR} ${PC_${COMPONENT}_LIBRARY_DIRS} REQUIRED
  )

  mark_as_advanced(${COMPONENT_NAME}_INCLUDE_DIR ${COMPONENT_NAME}_LIBRARY)

  list(APPEND XCB_INCLUDE_DIRS ${${COMPONENT_NAME}_INCLUDE_DIR})
  list(APPEND XCB_LIBRARIES ${${COMPONENT_NAME}_LIBRARY})
endforeach()

list(REMOVE_DUPLICATES XCB_INCLUDE_DIRS)

find_package_handle_standard_args(
  XCB
  FOUND_VAR XCB_FOUND
  REQUIRED_VARS XCB_LIBRARIES XCB_INCLUDE_DIRS
  HANDLE_COMPONENTS
)

message(STATUS ${XCB_INCLUDE_DIRS})
message(STATUS ${XCB_LIBRARIES})

set_package_properties(
  XCB PROPERTIES
  URL "https://xcb.freedesktop.org/"
  DESCRIPTION "X protocol C-language Binding"
)
