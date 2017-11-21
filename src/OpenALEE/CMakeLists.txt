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
ELSE()
    ADD_LIBRARY(CM.OpenALEE SHARED ${OpenALEESource})
ENDIF()

ADD_LIBRARY(CM.OpenALEE.StandAlone SHARED ${OpenALEESource} OpenALEECPort.cpp OpenALEEStandAlone.cpp ${CM_BASE_ALL_SOURCE})

IF(WIN32)
    target_compile_options(CM.OpenALEE.StandAlone PRIVATE "/MT")
    target_link_libraries(CM.OpenALEE.StandAlone CM.Base.Static CM.Platform.Static)
ENDIF(WIN32)
