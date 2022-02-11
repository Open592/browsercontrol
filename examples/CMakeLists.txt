# SPDX-License-Identifier: BSD-2-Clause

find_package(Java REQUIRED)
include(UseJava)

cmake_policy(SET CMP0070 NEW)

add_jar(exampleappletviewer
    SOURCES ExampleAppletViewer/ExampleAppletViewer.java
    SOURCES nativeadvert/browsercontrol.java
    ENTRY_POINT ExampleAppletViewer/ExampleAppletViewer
    OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

file(GENERATE OUTPUT "library-path-$<CONFIG>.txt" CONTENT "$<TARGET_FILE:browsercontrol>"
    TARGET browsercontrol
)
