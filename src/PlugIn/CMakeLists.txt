
# add_subdirectory(SystemCheck)

IF(BUILD_OPENAL_LIB)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/libogg/include)

    SET(OGG_SOURCE	${CMAKE_CURRENT_SOURCE_DIR}/libogg/src/bitwise.c
                	${CMAKE_CURRENT_SOURCE_DIR}/libogg/src/framing.c)
    
	add_subdirectory(Audio.Wav)
	add_subdirectory(Audio.Vorbis)
	add_subdirectory(Audio.Opus)
ENDIF(BUILD_OPENAL_LIB)
