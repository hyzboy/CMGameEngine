CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

OPTION(BUILD_BASE_LIB 			"Build Base Library"						TRUE	)
OPTION(BUILD_OPENGL_LIB			"Build OpenGL Library"						FALSE	)
OPTION(BUILD_OPENAL_LIB			"Build OpenAL Library"						FALSE	)
OPTION(BUILD_NETWORK_LIB		"Build Network Library"						TRUE	)
OPTION(BUILD_NETWORK_SCTP		"Include SCTP Support"						FALSE	)
OPTION(BUILD_QT4_SUPPORT_LIB	"Build QT4 Support Library"					FALSE	)
OPTION(BUILD_EXAMPLES_PROJECT	"Build Examples Project"					FALSE	)
OPTION(BUILD_TEST_PROJECT		"Build Test Project"						FALSE	)

OPTION(BUILD_GUI_TOOLS			"Build GUI Tools"							FALSE	)

set(CMGDK_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include(common.cmake)
include(cmgdk.cmake)

add_subdirectory(src)

IF(BUILD_TEST_PROJECT)
	add_subdirectory(test)
ENDIF(BUILD_TEST_PROJECT)

IF(BUILD_EXAMPLES_PROJECT)
	add_subdirectory(examples)
ENDIF(BUILD_EXAMPLES_PROJECT)

add_subdirectory(tools)
