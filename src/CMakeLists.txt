IF(BUILD_BASE_LIB)
	add_subdirectory(Base)
	add_subdirectory(UtilityTools)
ENDIF(BUILD_BASE_LIB)

IF(BUILD_SCRIPT_ANGEL)
    add_subdirectory(Script/AngelVM)
ENDIF()

IF(BUILD_SCRIPT_DEVIL)
    add_subdirectory(Script/DevilVM)
ENDIF()

IF(BUILD_NETWORK_LIB)
	add_subdirectory(Network)
ENDIF(BUILD_NETWORK_LIB)

add_subdirectory(SceneGraph)

IF(BUILD_OPENGL_LIB)
	add_subdirectory(RenderDevice)
ENDIF(BUILD_OPENGL_LIB)

IF(BUILD_OPENAL_LIB)
	add_subdirectory(OpenALEE)
ENDIF(BUILD_OPENAL_LIB)

add_subdirectory(DFS)

IF(UNIX)
	add_subdirectory(Platform/UNIX)
ENDIF(UNIX)

add_subdirectory(Platform)

add_subdirectory(PlugIn)

IF(UNIX)
	add_subdirectory(RedisClient)
ENDIF(UNIX)
