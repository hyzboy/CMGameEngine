 
# add_subdirectory(SystemCheck)

IF(BUILD_OPENAL_LIB)
	add_subdirectory(Audio.Wav)
	add_subdirectory(Audio.VorbisOGG)
ENDIF(BUILD_OPENAL_LIB)
