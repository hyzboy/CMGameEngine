
# add_subdirectory(SystemCheck)

IF(BUILD_OPENAL_LIB)
	add_subdirectory(Audio.Wav)
	add_subdirectory(Audio.Vorbis)
	add_subdirectory(Audio.Opus)
ENDIF(BUILD_OPENAL_LIB)
