SET(BASE_COMMON_SOURCE
	Base.cpp)

SOURCE_GROUP("Common" FILES ${BASE_COMMON_SOURCE})

SET(BASE_OTHER_SOURCE
	Other/ProbSelect.cpp
	Other/ExpendString.cpp
	Other/Other.cpp
	Other/Compress.cpp
	Other/File.cpp
	Other/MemoryPool.cpp
	Other/TimeCount.cpp
	Other/DateTime.cpp
	Other/PseudoRandom.cpp)

IF(UNIX)
	SET(BASE_OTHER_SOURCE ${BASE_OTHER_SOURCE} Other/TimePOSIX.cpp)
ENDIF(UNIX)

SET(BASE_OTHER_ALGORITHML_SOURCE
	Algorithml/Overflow.cpp
	Algorithml/CodePage.cpp
	Algorithml/LSinCos.cpp)

SOURCE_GROUP("Other"				FILES ${BASE_OTHER_SOURCE})
SOURCE_GROUP("Other\\Algorithml"	FILES ${BASE_OTHER_ALGORITHML_SOURCE})

SET(BASE_LOG_SOURCE
	LogInfo/Loginfo.cpp
	LogInfo/LogFile.cpp)

IF(WIN32)
	SET(BASE_OS_LOG_SOURCE
		LogInfo/LogWinConsole.cpp
		LogInfo/LogWinDialog.cpp)
ENDIF(WIN32)

IF(UNIX)
	SET(BASE_OS_LOG_SOURCE
		LogInfo/LogUnixConsole.cpp)
ENDIF(UNIX)

SOURCE_GROUP("LogInfo" FILES ${BASE_LOG_SOURCE} ${BASE_OS_LOG_SOURCE})

SET(BASE_APPLICATION_SOURCE
	Application/ConsoleSystemInitInfo.cpp
	Application/ConsoleApplication.cpp
	Application/BaseApplication.cpp
	Application/SystemInfo.cpp)

IF(BUILD_OPENGL_LIB)
	SET(BASE_APPLICATION_SOURCE	${BASE_APPLICATION_SOURCE} Application/GraphicsSystemInitInfo.cpp)
ENDIF(BUILD_OPENGL_LIB)

IF(UNIX)
SET(BASE_OS_APPLICATION_SOURCE
	Application/SystemInfoUNIX.cpp)
ENDIF(UNIX)

IF(WIN32)
SET(BASE_OS_APPLICATION_SOURCE
	Application/SystemInfoWinNT.cpp)
ENDIF(WIN32)

SOURCE_GROUP("Application" FILES ${BASE_APPLICATION_SOURCE} ${BASE_OS_APPLICATION_SOURCE})

SET(BASE_TYPE_SOURCE
	DataType/DataType.cpp
	DataType/int128.cpp
	DataType/BitArray.cpp
	DataType/Color.cpp
	DataType/Color3f.cpp
	DataType/Color4f.cpp
	DataType/Font.cpp
    DataType/StringList.cpp)

SET(BASE_OPENGL_TYPE_SOURCE
	DataType/ColorType.cpp)

IF(BUILD_OPENGL_LIB)
	SET(BASE_TYPE_SOURCE ${BASE_TYPE_SOURCE} ${BASE_OPENGL_TYPE_SOURCE})
ENDIF(BUILD_OPENGL_LIB)

SOURCE_GROUP("DataType" FILES ${BASE_TYPE_SOURCE})

SET(BASE_IO_SOURCE
	IO/FileOutputStream.cpp
	IO/JavaOutputStream.cpp
	IO/RandomAccessFile.cpp
	IO/DataOutputStream.cpp
	IO/FileInputStream.cpp
	IO/DataInputStream.cpp
	IO/FileAccess.cpp
	IO/JavaInputStream.cpp
	IO/TextOutputStream.cpp
	IO/DataDistribute.cpp
	IO/IOType.cpp)

SOURCE_GROUP("IO" FILES ${BASE_IO_SOURCE})

SET(BASE_PLUG_IN_SOURCE
	PlugIn/ExternalModule.cpp
	PlugIn/PlugIn.cpp
)

SOURCE_GROUP("PlugIn" FILES ${BASE_PLUG_IN_SOURCE})

SET(BASE_MULTI_PROCESS_SOURCE
	MultiProcess/SharedMemory.cpp
	MultiProcess/ProcMutexAPR.cpp)

SET(BASE_MULTI_THREAD_SOURCE
	MultiThread/CondVar.cpp
	MultiThread/RingBuffer.cpp
	MultiThread/RingBufferRead.cpp
	MultiThread/RingBufferWrite.cpp)

IF(UNIX)
	SET(BASE_MULTI_THREAD_SOURCE ${BASE_MULTI_THREAD_SOURCE}
		MultiThread/Posix/CondVarPOSIX.cpp
		MultiThread/Posix/RWLockPOSIX.cpp
		MultiThread/Posix/SemaphorePOSIX.cpp
		MultiThread/Posix/ThreadPOSIX.cpp
		MultiThread/Posix/ThreadMutexPOSIX.cpp)

	SET(BASE_MULTI_PROCESS_SOURCE ${BASE_MULTI_PROCESS_SOURCE}
		MultiProcess/ProcessPOSIX.cpp
		MultiProcess/Pipe.cpp
		MultiProcess/Fifo.cpp)
ENDIF(UNIX)

IF(WIN32)
	SET(BASE_MULTI_THREAD_SOURCE ${BASE_MULTI_THREAD_SOURCE}
		MultiThread/APR/RWLockAPR.cpp
		MultiThread/WinNT/CondVarNT6.cpp
		MultiThread/WinNT/RWLockNT6.cpp
		MultiThread/WinNT/SemaphoreNT4.cpp
		MultiThread/WinNT/ThreadNT4.cpp
		MultiThread/WinNT/ThreadMutexNT4.cpp)
ENDIF(WIN32)

SOURCE_GROUP("MultiThread"	FILES ${BASE_MULTI_THREAD_SOURCE})
SOURCE_GROUP("MultiProcess"	FILES ${BASE_MULTI_PROCESS_SOURCE})

ADD_LIBRARY(CM.Base SHARED
			${BASE_COMMON_SOURCE}
			${BASE_OTHER_SOURCE}
			${BASE_OTHER_ALGORITHML_SOURCE}
			${BASE_LOG_SOURCE}
			${BASE_OS_LOG_SOURCE}
			${BASE_APPLICATION_SOURCE}
			${BASE_OS_APPLICATION_SOURCE}
			${BASE_TYPE_SOURCE}
			${BASE_IO_SOURCE}
			${BASE_PLUG_IN_SOURCE}
			${BASE_MULTI_PROCESS_SOURCE}
			${BASE_MULTI_THREAD_SOURCE})
