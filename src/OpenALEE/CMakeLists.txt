Set(OpenALEESource
	al.cpp
	alc.cpp
	AudioFileType.cpp
	AudioBuffer.cpp
	AudioDecode.cpp
	AudioManage.cpp
	AudioScene.cpp
	AudioPlayer.cpp
	AudioSource.cpp
	EFX.cpp
	Listener.cpp
	OpenAL.cpp
	XRAM.cpp)

IF(WIN32)
	ADD_LIBRARY(CM.OpenALEE STATIC ${OpenALEESource})
	ADD_LIBRARY(CM.OpenALEEStandAlone STATIC ${OpenALEESource} OpenALEEStandAlone.cpp)
ELSE()
	ADD_LIBRARY(CM.OpenALEE SHARED ${OpenALEESource})
	ADD_LIBRARY(CM.OpenALEEStandAlone SHARED ${OpenALEESource} OpenALEEStandAlone.cpp)
ENDIF()
