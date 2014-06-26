CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

set(CMGDK_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include(cmgdk.cmake REQUIRE)

add_subdirectory(src)

IF(BUILD_TEST_PROJECT)
	add_subdirectory(test)
ENDIF(BUILD_TEST_PROJECT)

IF(BUILD_EXAMPLES_PROJECT)
	add_subdirectory(examples)
ENDIF(BUILD_EXAMPLES_PROJECT)

add_subdirectory(tools)
