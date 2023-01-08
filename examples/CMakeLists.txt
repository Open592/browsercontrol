# SPDX-License-Identifier: BSD-2-Clause

find_package(Java REQUIRED)
include(UseJava)

get_property(generatorIsMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

add_jar(examplejar
        SOURCES ExampleAppletViewer/AdvertComponentListener.java
        SOURCES ExampleAppletViewer/AdvertFrame.java
        SOURCES ExampleAppletViewer/ExampleAppletViewer.java
        SOURCES ExampleAppletViewer/TerminateHandler.java
        SOURCES nativeadvert/browsercontrol.java
        ENTRY_POINT ExampleAppletViewer/ExampleAppletViewer
        OUTPUT_NAME example
        OUTPUT_DIR ${OUTPUT_DIR}
        )

if (generatorIsMultiConfig)
    get_target_property(EXAMPLE_JAR_LOCATION examplejar JAR_FILE)

    add_custom_command(TARGET examplejar POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${EXAMPLE_JAR_LOCATION} $<TARGET_FILE_DIR:browsercontrol>
            DEPENDS $<TARGET_FILE_DIR:browsercontrol>
            COMMENT "Copy JAR file to resulting build directory for multi config generators"
            VERBATIM
            )
endif ()
