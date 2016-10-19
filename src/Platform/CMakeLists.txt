IF(APPLE)
	add_library(OSXGUI Clipboard.cpp)
ENDIF(APPLE)

IF(BUILD_QT5_SUPPORT_LIB)
	add_subdirectory(QT)
ENDIF()

IF(UNIX)

    SET(BASE_OS_APPLICATION_SOURCE UNIX/SystemInfo.cpp)

	SET(BASE_OS_LOG_SOURCE         UNIX/LogConsole.cpp)

	SET(BASE_MULTI_THREAD_SOURCE
		UNIX/CondVar.cpp
		UNIX/RWLock.cpp
		UNIX/Semaphore.cpp
		UNIX/Thread.cpp
		UNIX/ThreadMutex.cpp)

	SET(BASE_MULTI_PROCESS_SOURCE
		UNIX/ProcMutex.cpp
		UNIX/Process.cpp
		UNIX/Pipe.cpp
		UNIX/Fifo.cpp)

    SET(BASE_TIME_SOURCE    UNIX/Time.cpp
                            UNIX/Exit.cpp
                            UNIX/DateTime.cpp)
ENDIF(UNIX)

IF(WIN32)
    SET(BASE_OS_APPLICATION_SOURCE Win/SystemInfo.cpp)

	SET(BASE_OS_LOG_SOURCE         Win/LogConsole.cpp
                                   Win/LogDialog.cpp)

	SET(BASE_MULTI_THREAD_SOURCE
		Win/CondVar.cpp
		Win/RWLock.cpp
		Win/Semaphore.cpp
		Win/Thread.cpp
		Win/ThreadMutex.cpp)

	SET(BASE_MULTI_PROCESS_SOURCE
		Win/ProcMutex.cpp)

    SET(BASE_TIME_SOURCE    Win/Time.cpp
                            Win/DateTime.cpp)
ENDIF(WIN32)

SOURCE_GROUP("SystemInfo"   FILES ${BASE_OS_APPLICATION_SOURCE})
SOURCE_GROUP("LogInfo"      FILES ${BASE_OS_LOG_SOURCE})
SOURCE_GROUP("MultiThread"  FILES ${BASE_MULTI_THREAD_SOURCE})
SOURCE_GROUP("MultiProcess" FILES ${BASE_MULTI_PROCESS_SOURCE})
SOURCE_GROUP("Time"         FILES ${BASE_TIME_SOURCE})

SET(PLATFORM_SOURCE ${PLATFORM_SOURCE}
                    ${BASE_TIME_SOURCE}
                    ${BASE_OS_APPLICATION_SOURCE}
                    ${BASE_OS_LOG_SOURCE}
                    ${BASE_MULTI_THREAD_SOURCE}
                    ${BASE_MULTI_PROCESS_SOURCE})

IF(BUILD_OPENGL_LIB)

    SET(PLATFORM_SOURCE ${PLATFORM_SOURCE}
                        Window.cpp)

    SOURCE_GROUP("Graphics" FILES Window.cpp ${PLATFORM_SOURCE})

    SET(PLATFORM_GLFW_SUPPORT_SOURCE    GLFW/PlatformGLFW.cpp
                                        GLFW/WindowGLFW.cpp)

    SET(PLATFORM_SOURCE ${PLATFORM_SOURCE}
                        ${PLATFORM_GLFW_SUPPORT_SOURCE})

    SOURCE_GROUP("Graphics" FILES ${PLATFORM_GLFW_SUPPORT_SOURCE})

    IF(WIN32)
        add_definitions("-D_GLFW_WIN32")
        add_definitions("-D_GLFW_WGL")

        set(GLFW_SOURCE		GLFW/GLFW3/context.c
                            GLFW/GLFW3/egl_context.c
                            GLFW/GLFW3/init.c
                            GLFW/GLFW3/input.c
                            GLFW/GLFW3/monitor.c
                            GLFW/GLFW3/vulkan.c
                            GLFW/GLFW3/wgl_context.c
                            GLFW/GLFW3/win32_init.c
                            GLFW/GLFW3/win32_joystick.c
                            GLFW/GLFW3/win32_monitor.c
                            GLFW/GLFW3/win32_time.c
                            GLFW/GLFW3/win32_tls.c
                            GLFW/GLFW3/win32_window.c
                            GLFW/GLFW3/window.c)

        SOURCE_GROUP("Graphics\\GLFW3"   FILES ${GLFW_SOURCE})

        SET(PLATFORM_SOURCE ${PLATFORM_SOURCE}
                            ${GLFW_SOURCE})
    ENDIF(WIN32)
ENDIF(BUILD_OPENGL_LIB)

add_library(CM.Platform STATIC ${PLATFORM_SOURCE})

