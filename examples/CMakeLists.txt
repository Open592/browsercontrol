# SPDX-License-Identifier: BSD-2-Clause

find_package(Java REQUIRED)
include(UseJava)

add_jar(exampleappletviewer
    SOURCES ExampleAppletViewer/ExampleAppletViewer.java
    SOURCES nativeadvert/browsercontrol.java
    ENTRY_POINT ExampleAppletViewer/ExampleAppletViewer
    OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
