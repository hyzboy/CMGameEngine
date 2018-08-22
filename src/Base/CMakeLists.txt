SET(BASE_COMMON_SOURCE
    Base.cpp)

SOURCE_GROUP("Common" FILES ${BASE_COMMON_SOURCE})

SET(BASE_OBJECT_SOURCE)
#     Object/ConsoleObjectControl.cpp)

SOURCE_GROUP("Object"    FILES ${BASE_OBJECT_SOURCE})

SET(BASE_OTHER_SOURCE
    Other/ExpendString.cpp
    Other/FileSystem.cpp
    Other/TimeCount.cpp
	Other/TimeVal.cpp
    Other/DateTime.cpp
    Other/CodePage.cpp
    Other/EnumFile.cpp)

SOURCE_GROUP("Other"                FILES ${BASE_OTHER_SOURCE})

SET(BASE_LOG_SOURCE
    LogInfo/Loginfo.cpp
    LogInfo/LogFile.cpp)

SOURCE_GROUP("LogInfo" FILES ${BASE_LOG_SOURCE})

SET(BASE_APPLICATION_SOURCE
    Application/ConsoleSystemInitInfo.cpp
    Application/ConsoleApplication.cpp
    Application/BaseApplication.cpp
    Application/SystemInfo.cpp)

SOURCE_GROUP("Application" FILES ${BASE_APPLICATION_SOURCE})

SET(BASE_TYPE_SOURCE
    DataType/Info.cpp
    DataType/BitArray.cpp
    DataType/Color.cpp
    DataType/Color3f.cpp
    DataType/Color4f.cpp
    DataType/FontInfo.cpp
    DataType/StringList.cpp)

SOURCE_GROUP("DataType" FILES ${BASE_TYPE_SOURCE})

SET(BASE_IO_SOURCE
    IO/FileOutputStream.cpp
    IO/RandomAccessFile.cpp
    IO/DataOutputStream.cpp
    IO/FileInputStream.cpp
    IO/DataInputStream.cpp
    IO/FileAccess.cpp
    IO/TextOutputStream.cpp
    IO/DataDistribute.cpp
    IO/IOType.cpp)

SOURCE_GROUP("IO" FILES ${BASE_IO_SOURCE})

SET(THREAD_SOURCE   MultiThread/WorkThread.cpp
                    MultiThread/WorkThreadTeam.cpp)

SOURCE_GROUP("MultiThread" FILES ${THREAD_SOURCE})

SET(BASE_PLUG_IN_SOURCE
    PlugIn/ExternalModule.cpp
    PlugIn/PlugIn.cpp
)

SOURCE_GROUP("PlugIn" FILES ${BASE_PLUG_IN_SOURCE})

SET(CM_BASE_ALL_SOURCE  ${BASE_COMMON_SOURCE}
                        ${BASE_OBJECT_SOURCE}
                        ${BASE_OTHER_SOURCE}
                        ${BASE_LOG_SOURCE}
                        ${BASE_APPLICATION_SOURCE}
                        ${BASE_TYPE_SOURCE}
                        ${BASE_IO_SOURCE}
                        ${THREAD_SOURCE}
                        ${BASE_PLUG_IN_SOURCE})

IF(WIN32)
    ADD_LIBRARY(CM.Base STATIC ${CM_BASE_ALL_SOURCE})

    ADD_LIBRARY(CM.Base.Static STATIC ${CM_BASE_ALL_SOURCE})

    target_compile_options(CM.Base.Static PRIVATE "/MT")
ELSE()
    ADD_LIBRARY(CM.Base SHARED ${CM_BASE_ALL_SOURCE})
ENDIF()
