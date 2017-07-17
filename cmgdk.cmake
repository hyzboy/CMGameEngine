SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)

OPTION(BUILD_BASE_LIB 			"Build Base Library"						TRUE	)
OPTION(BUILD_ALGORITHM          "Build Algorithm (Hash/Crypt)"              FALSE   )

OPTION(BUILD_DATABASE           "Build Database support"                    FALSE   )

OPTION(BUILD_NETWORK_LIB		"Build Network Library"						TRUE	)

OPTION(BUILD_SCRIPT_ANGEL       "Build Script AngelScript"                  FALSE   )
OPTION(BUILD_SCRIPT_DEVIL       "Build Script DevilScript"                  FALSE   )

OPTION(BUILD_NETWORK_SCTP		"Include SCTP Support"						FALSE	)
OPTION(BUILD_NETWORK_UDP_LITE   "Include UDP-Lite Support"                  FALSE   )
OPTION(BUILD_QT5_SUPPORT_LIB	"Build QT5 Support Library"					FALSE	)
OPTION(BUILD_EXAMPLES_PROJECT	"Build Examples Project"					FALSE	)
OPTION(BUILD_TEST_PROJECT		"Build Test Project"						FALSE	)
OPTION(BUILD_GUI_TOOLS			"Build GUI Tools"							FALSE	)

OPTION(BUILD_OpenCart           "Build OpenCart Tools"                      FALSE   )
OPTION(LOG_CDB_LOADER_LOG		"Output CDBLoader log"						FALSE	)

if(LOG_CDB_LOADER_LOG)
	add_definitions("-DLOG_CDB_LOADER_LOG")
endif(LOG_CDB_LOADER_LOG)

INCLUDE_DIRECTORIES(${CMGDK_PATH}/inc)
INCLUDE_DIRECTORIES(${CMGDK_PATH}/3rdpty)

IF(WIN32)
    INCLUDE_DIRECTORIES(${CMGDK_PATH}/inc/win)
ENDIF(WIN32)

ADD_DEFINITIONS("-DMATH_USE_OPENGL")
ADD_DEFINITIONS("-DMATH_RIGHTHANDED_CAMERA")

INCLUDE_DIRECTORIES(${CMGDK_PATH}/3rdpty/MathGeoLib/src)

IF(WIN32)
    link_directories(${CMGDK_PATH}/3rdpty/MathGeoLib/${CMGDK_BUILD_TYPE})
ELSE(WIN32)
    link_directories(${CMGDK_PATH}/3rdpty/MathGeoLib)
ENDIF(WIN32)

IF(UNIX)
	FIND_PATH(ICONV_INCLUDE_DIR
		NAMES iconv.h
		HINTS
		/usr/include
		/usr/local/include)
	INCLUDE_DIRECTORIES(${ICONV_INCLUDE_DIR})

	IF(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	FIND_PATH(ICONV_LIBRARY_DIR
		NAMES libiconv.so
		HINTS
		${LIB_3RD_FIND_HINT})
	LINK_DIRECTORIES(${ICONV_LIBRARY_DIR})
	ENDIF(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")

# 	FIND_PATH(MYSQL_INCLUDE_DIR
# 		NAMES mysql.h
# 		HINTS
# 		/usr/include
# 		/usr/include/mysql
# 		/usr/local/include
# 		/usr/local/include/mysql
# 		/usr/mysql/include/mysql)				#solaris 目录
# 	INCLUDE_DIRECTORIES(${MYSQL_INCLUDE_DIR})
#
# 	FIND_PATH(MYSQL_LIBRARY_DIR
# 		NAMES libmysqlclient.so
# 		HINTS
# 		${LIB_3RD_FIND_HINT}
# 		/usr/lib${HGL_BITS}/mysql
# 		/usr/local/lib${HGL_BITS}/mysql
# 		/usr/lib/mysql
# 		/usr/local/lib/mysql
#
# 		/usr/mysql/lib/${CMAKE_SYSTEM_PROCESSOR}/mysql	#solaris amd64 目录
# 		/usr/mysql/lib/mysql)
# 	LINK_DIRECTORIES(${MYSQL_LIBRARY_DIR})
ENDIF()

SET(HGL_BASE_LIB CM.Base CM.UT MathGeoLib)

IF(BUILD_ALGORITHM)
    SET(HGL_BASE_LIB ${HGL_BASE_LIB} CM.Algorithm)
ENDIF(BUILD_ALGORITHM)

IF(BUILD_DATABASE)
    SET(HGL_BASE_LIB ${HGL_BASE_LIB} CM.Database)
ENDIF(BUILD_DATABASE)

IF(BUILD_NETWORK_LIB)
	SET(HGL_NETWORK_LIB CM.Network)
ENDIF(BUILD_NETWORK_LIB)

IF(BUILD_QT5_SUPPORT_LIB)

    add_definitions("-DHGL_QT5")

    set(QT_MIN_VERSION "5.3.0")
    find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core)

    include_directories(${Qt5Core_INCLUDES})
    add_definitions(${Qt5Core_DEFINITIONS})

	IF(WIN32)
		SET(HGL_QT_MAIN_SOURCE ${CMGDK_PATH}/src/Platform/QT/PlatformQT5WinUTF16.cpp)
	ELSE()
		SET(HGL_QT_MAIN_SOURCE ${CMGDK_PATH}/src/Platform/QT/PlatformQT5UnixUTF8.cpp)
	ENDIF()

	SET(HGL_QT_LIB CM.QT5Support)
ENDIF()

IF(UNIX)
	MESSAGE("Host OS is UNIX")

	SET(HGL_CONSOLE_MAIN_SOURCE ${CMGDK_PATH}/src/Platform/UNIX/MainConsole.cpp)
	SET(HGL_GRAPHICS_MAIN_SOURCE ${CMGDK_PATH}/src/Platform/UNIX/MainOpenGL.cpp)

	SET(HGL_BASE_LIB ${HGL_BASE_LIB} pthread dl rt)

	IF(USE_ELECTRIC_FENCE)
		SET(HGL_BASE_LIB ${HGL_BASE_LIB} efence)
	ENDIF()

	IF(USE_GPERF_TOOLS)
		SET(HGL_BASE_LIB ${HGL_BASE_LIB} tcmalloc)
	ENDIF(USE_GPERF_TOOLS)

	IF(${CMAKE_SYSTEM_NAME} MATCHES ".*Linux.*")
		IF(ANDROID)
			MESSAGE("Set Android HGL_BASE_LIB")
		ELSE(ANDROID)
			MESSAGE("Set Linux HGL_BASE_LIB")
		ENDIF(ANDROID)
	ENDIF()

	IF(${CMAKE_SYSTEM_NAME} MATCHES ".*MacOS.*")
		MESSAGE("Set MacOS HGL_BASE_LIB")
		SET(HGL_BASE_LIB ${HGL_BASE_LIB} iconv)
	ENDIF()

	IF(${CMAKE_SYSTEM_NAME} MATCHES ".*FreeBSD.*")
		MESSAGE("Set FreeBSD HGL_BASE_LIB")
		SET(HGL_BASE_LIB ${HGL_BASE_LIB} iconv)
	ENDIF()

	IF(${CMAKE_SYSTEM_NAME} MATCHES ".*SunOS")
		MESSAGE("Set Solaris HGL_BASE_LIB")
		SET(HGL_BASE_LIB ${HGL_BASE_LIB} socket nsl)
	ENDIF()

	IF(BUILD_NETWORK_SCTP)
		SET(HGL_NETWORK_LIB ${HGL_NETWORK_LIB} sctp)
		add_definitions("-DHGL_NETWORK_SCTP_SUPPORT")
	ENDIF(BUILD_NETWORK_SCTP)

	SET(HGL_CONSOLE_LIB ${HGL_BASE_LIB} ${HGL_NETWORK_LIB} CM.SceneGraph CM.Platform expat)
	SET(HGL_GRAPHICS_LIB ${HGL_BASE_LIB} ${HGL_NETWORK_LIB} CM.SceneGraphRender CM.RenderDevice CM.PlatformRender ${HGL_OpenGL_LIB} X11 Xxf86vm Xrandr Xcursor Xinerama Xi expat)

	IF(OPENGL_USE_GLFW)
        SET(HGL_GRAPHICS_LIB ${HGL_GRAPHICS_LIB} glfw)
    ENDIF(OPENGL_USE_GLFW)
ELSE(UNIX)
	MESSAGE("Host OS don't is UNIX")
ENDIF(UNIX)

IF(WIN32)
	MESSAGE("Host OS is Windows")

	SET(HGL_CONSOLE_MAIN_SOURCE ${CMGDK_PATH}/src/Platform/Win/WinMainConsole.cpp)
	SET(HGL_GRAPHICS_MAIN_SOURCE ${CMGDK_PATH}/src/Platform/Win/WinMainOpenGL.cpp)
	SET(HGL_CONSOLE_LIB ${HGL_BASE_LIB} ${HGL_NETWORK_LIB} CM.SceneGraph CM.Platform ws2_32.lib)
	SET(HGL_GRAPHICS_LIB ${HGL_BASE_LIB} ${HGL_NETWORK_LIB} CM.SceneGraphRender CM.RenderDevice CM.PlatformRender ${HGL_OpenGL_LIB} OpenGL32.lib ws2_32.lib)

	SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:msvcrt.lib ")
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libcmtd.lib;libcmt.lib")

ENDIF(WIN32)

IF (APPLE)
        SET(HGL_GUI_TYPE MACOSX_BUNDLE)
ELSEIF(WIN32)
        SET(HGL_GUI_TYPE WIN32)     
ELSEIF(UNIX)
        SET(HGL_GUI_TYPE)
ENDIF()

message("")
message("CMGDK_BUILD_TYPE: " ${CMGDK_BUILD_TYPE})
message("CMAKE_SIZEOF_VOID_P = ${CMAKE_SIZEOF_VOID_P}")
message("")
MESSAGE("HGL_BASE_LIB: " ${HGL_BASE_LIB})
MESSAGE("HGL_NETWORK_LIB: " ${HGL_NETWORK_LIB})
MESSAGE("HGL_CONSOLE_LIB: " ${HGL_CONSOLE_LIB})
MESSAGE("HGL_GRAPHICS_LIB: " ${HGL_GRAPHICS_LIB})
MESSAGE("HGL_CONSOLE_MAIN_SOURCE: " ${HGL_CONSOLE_MAIN_SOURCE})
MESSAGE("HGL_GRAPHICS_MAIN_SOURCE: " ${HGL_GRAPHICS_MAIN_SOURCE})
MESSAGE("HGL_GUI_MAIN_SOURCE: " ${HGL_GUI_MAIN_SOURCE})
message("")
MESSAGE("Sytem: " ${CMAKE_SYSTEM})
MESSAGE("System Name: " ${CMAKE_SYSTEM_NAME})
MESSAGE("System Version: " ${CMAKE_SYSTEM_VERSION})
MESSAGE("System Processor: " ${CMAKE_SYSTEM_PROCESSOR})

MESSAGE("Host System : " ${CMAKE_HOST_SYSTEM})
MESSAGE("Host System Name: " ${CMAKE_HOST_SYSTEM_NAME})
MESSAGE("Host System Version: " ${CMAKE_HOST_SYSTEM_VERSION})
MESSAGE("Host System Processor: " ${CMAKE_HOST_SYSTEM_PROCESSOR})

MESSAGE("Generator: " ${CMAKE_GENERATOR})
MESSAGE("Generator Platform: " ${CMAKE_GENERATOR_PLATFORM})
MESSAGE("Generator Toolset: " ${CMAKE_GENERATOR_TOOLSET})
message("")
MESSAGE("C Compiler: " ${CMAKE_C_COMPILER})
MESSAGE("C++ Compiler: " ${CMAKE_CXX_COMPILER})
message("")
MESSAGE("C Flags: " ${CMAKE_C_FLAGS})
MESSAGE("C++ Flags: " ${CMAKE_CXX_FLAGS})
MESSAGE("C Debug Flags: " ${CMAKE_C_FLAGS_DEBUG})
MESSAGE("C++ Debug Flags: " ${CMAKE_CXX_FLAGS_DEBUG})
MESSAGE("C Release Flags: " ${CMAKE_C_FLAGS_RELEASE})
MESSAGE("C++ Release Flags: " ${CMAKE_CXX_FLAGS_RELEASE})

