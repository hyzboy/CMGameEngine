Set(OpenALEESource
	al.cpp
	alc.cpp
	AudioFileType.cpp
	AudioBuffer.cpp
	AudioDecode.cpp
	AudioManage.cpp
	AudioPlayer.cpp
	AudioSource.cpp
	EFX.cpp
	Listener.cpp
	OpenAL.cpp
	XRAM.cpp)

ADD_LIBRARY(CM.OpenALEE SHARED ${OpenALEESource})

ADD_LIBRARY(CM.OpenALEEStandAlone SHARED ${OpenALEESource} OpenALEEStandAlone.cpp)
