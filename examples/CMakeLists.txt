# SPDX-License-Identifier: BSD-2-Clause

find_package(Java REQUIRED)
include(UseJava)

get_property(generatorIsMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

set(EXAMPLES_OUTPUT_DIR ${OUTPUT_DIR})

# When in multi config environment we will copy the output jar to the appropriate build directory. Because of this we
# will store the jar in its own directory to avoid polluting the root.
if(generatorIsMultiConfig)
  set(EXAMPLES_OUTPUT_DIR ${OUTPUT_DIR}/examples)
endif()

set(APPLETVIEWER_DIRECTORY "AppletViewer/src/main/java/com/open592/appletviewer")
add_jar(
  examplejar
  SOURCES ${APPLETVIEWER_DIRECTORY}/AdvertComponentListener.java
  SOURCES ${APPLETVIEWER_DIRECTORY}/AdvertFrame.java
  SOURCES ${APPLETVIEWER_DIRECTORY}/AppletViewer.java
  SOURCES ${APPLETVIEWER_DIRECTORY}/TerminateHandler.java
  SOURCES AppletViewer/src/main/java/nativeadvert/browsercontrol.java
  ENTRY_POINT com.open592.appletviewer.AppletViewer
  OUTPUT_NAME example
  OUTPUT_DIR ${EXAMPLES_OUTPUT_DIR}
)

if(generatorIsMultiConfig)
  get_target_property(EXAMPLE_JAR_LOCATION examplejar JAR_FILE)

  add_custom_command(
    TARGET examplejar
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${EXAMPLE_JAR_LOCATION} $<TARGET_FILE_DIR:browsercontrol> DEPENDS
            $<TARGET_FILE_DIR:browsercontrol>
    COMMENT "Copy JAR file to resulting build directory for multi config generators"
    VERBATIM
  )
endif()
