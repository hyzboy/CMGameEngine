IF(APPLE)
    add_library(OSXGUI Clipboard.cpp)
ENDIF(APPLE)

IF(BUILD_QT5_SUPPORT_LIB)
    add_subdirectory(QT)
ENDIF()

IF(UNIX)

    SET(PLATFORM_FILE_SOURCE            UNIX/File.cpp
                                        UNIX/FileAccess.cpp)

    SET(PLATFORM_CODEPAGE_SOURCE        UNIX/CodePage.cpp)

    SET(PLATFORM_APPLICATION_SOURCE     UNIX/SystemInfo.cpp)

    SET(PLATFORM_LOG_SOURCE             UNIX/LogConsole.cpp
                                        UNIX/LogFile.cpp)

    SET(PLATFORM_MULTI_THREAD_SOURCE    UNIX/CondVar.cpp
                                        UNIX/RWLock.cpp
                                        UNIX/Semaphore.cpp
                                        UNIX/Thread.cpp
                                        UNIX/ThreadMutex.cpp)

    SET(PLATFORM_MULTI_PROCESS_SOURCE   UNIX/ProcMutex.cpp
                                        UNIX/Process.cpp
                                        UNIX/Pipe.cpp
                                        UNIX/Fifo.cpp)

    SET(PLATFORM_TIME_SOURCE            UNIX/Time.cpp
                                        UNIX/Exit.cpp
                                        UNIX/DateTime.cpp)

    SET(PLATFORM_EXTERNAL_MODULE_SOURCE UNIX/ExternalModule.cpp)
ENDIF(UNIX)

IF(WIN32)

    SET(PLATFORM_FILE_SOURCE            Win/File.cpp
                                        Win/FileAccess.cpp)

    SET(PLATFORM_CODEPAGE_SOURCE        Win/CodePage.cpp)

    SET(PLATFORM_APPLICATION_SOURCE     Win/SystemInfo.cpp)

    SET(PLATFORM_LOG_SOURCE             Win/LogConsole.cpp
                                        Win/LogDialog.cpp
                                        Win/LogFile.cpp)

    SET(PLATFORM_MULTI_THREAD_SOURCE
                                        Win/CondVar.cpp
                                        Win/RWLock.cpp
                                        Win/Semaphore.cpp
                                        Win/Thread.cpp
                                        Win/ThreadMutex.cpp)

    SET(PLATFORM_MULTI_PROCESS_SOURCE   Win/ProcMutex.cpp)

    SET(PLATFORM_TIME_SOURCE            Win/Time.cpp
                                        Win/DateTime.cpp)

    SET(PLATFORM_EXTERNAL_MODULE_SOURCE Win/ExternalModule.cpp)

    SET(PLATFORM_DESKTOP_SOURCE         Win/Desktop.cpp)
ENDIF(WIN32)

SOURCE_GROUP("CodePage"     FILES ${PLATFORM_CODEPAGE_SOURCE})
SOURCE_GROUP("File"         FILES ${PLATFORM_FILE_SOURCE})
SOURCE_GROUP("SystemInfo"   FILES ${PLATFORM_APPLICATION_SOURCE})
SOURCE_GROUP("LogInfo"      FILES ${PLATFORM_LOG_SOURCE})
SOURCE_GROUP("MultiThread"  FILES ${PLATFORM_MULTI_THREAD_SOURCE})
SOURCE_GROUP("MultiProcess" FILES ${PLATFORM_MULTI_PROCESS_SOURCE})
SOURCE_GROUP("Time"         FILES ${PLATFORM_TIME_SOURCE})
SOURCE_GROUP("Desktop"      FILES ${PLATFORM_DESKTOP_SOURCE})

IF(BUILD_OPENGL_LIB)

    SET(PLATFORM_SOURCE ${PLATFORM_SOURCE}
                        Window.cpp)

    SOURCE_GROUP("Graphics" FILES Window.cpp)

    IF(USE_GLFW)
        SET(PLATFORM_GLFW_SUPPORT_SOURCE    GLFW/PlatformGLFW.cpp
                                            GLFW/WindowGLFW.cpp
                                            GLFW/WindowEventGLFW.cpp
                                            GLFW/KeyConvert.cpp)

        SET(PLATFORM_SOURCE ${PLATFORM_SOURCE}
                            ${PLATFORM_GLFW_SUPPORT_SOURCE})

        SOURCE_GROUP("Graphics\\Platform" FILES ${PLATFORM_GLFW_SUPPORT_SOURCE})

        IF(WIN32)
            add_definitions("-D_GLFW_WIN32")
            add_definitions("-D_GLFW_WGL")

            set(GLFW_SOURCE     GLFW/GLFW3/context.c
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
    ELSE(USE_GLFW)
    
        IF(WIN32)
            SET(PLATFORM_INPUT_DEVICE_SOURCE    Win/KeyConvert.cpp)
        ENDIF(WIN32)
    
        IF(UNIX)
            set(X11_PLATFORM_SOURCE X11/X11Window.cpp)

            SOURCE_GROUP("Graphics\\X11" FILES ${X11_PLATFORM_SOURCE})

            SET(PLATFORM_SOURCE ${PLATFORM_SOURCE}
                                ${X11_PLATFORM_SOURCE})
        ENDIF(UNIX)
        
        IF(APPLE)
        ENDIF(APPLE)
    ENDIF(USE_GLFW)
    
    SOURCE_GROUP("InputDevice"  FILES ${PLATFORM_INPUT_DEVICE_SOURCE})
ENDIF(BUILD_OPENGL_LIB)

SET(PLATFORM_SOURCE ${PLATFORM_SOURCE}
                    ${PLATFORM_FILE_SOURCE}
                    ${PLATFORM_CODEPAGE_SOURCE}
                    ${PLATFORM_APPLICATION_SOURCE}
                    ${PLATFORM_LOG_SOURCE}
                    ${PLATFORM_MULTI_THREAD_SOURCE}
                    ${PLATFORM_MULTI_PROCESS_SOURCE}
                    ${PLATFORM_TIME_SOURCE}
                    ${PLATFORM_EXTERNAL_MODULE_SOURCE}
                    ${PLATFORM_DESKTOP_SOURCE}
                    ${PLATFORM_INPUT_DEVICE_SOURCE})
                    
add_library(CM.Platform STATIC ${PLATFORM_SOURCE})

