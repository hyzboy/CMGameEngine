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

ADD_LIBRARY(CM.OpenALEEStandAlone SHARED ${OpenALEESource} OpenALEECPort.cpp OpenALEEStandAlone.cpp ${CM_BASE_ALL_SOURCE})

target_compile_options(CM.OpenALEEStandAlone PRIVATE "/MT")

target_link_libraries(CM.OpenALEEStandAlone CM.Base.Static CM.Platform.Static)
