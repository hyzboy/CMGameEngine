CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(CMGDK VERSION 50)

set(CMGDK_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# configure_file(
#     "${CMAKE_CURRENT_SOURCE_DIR}/inc/hgl/platform/config.h.in"
#     "${CMAKE_CURRENT_SOURCE_DIR}/inc/hgl/platform/config.h"
# )

include(common.cmake REQUIRED)
include(math.cmake REQUIRED)
include(openal.cmake REQUIRED)
include(opengl.cmake REQUIRED)
include(loginfo.cmake REQUIRED)
include(cmgdk.cmake REQUIRED)

add_subdirectory(src)

IF(BUILD_TEST_PROJECT)
    add_subdirectory(test)
ENDIF(BUILD_TEST_PROJECT)

IF(BUILD_EXAMPLES_PROJECT)
    add_subdirectory(examples)
ENDIF(BUILD_EXAMPLES_PROJECT)

IF(BUILD_TOOLS)
    add_subdirectory(tools)
ENDIF()
