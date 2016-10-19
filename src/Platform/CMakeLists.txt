IF(APPLE)
	add_library(OSXGUI Clipboard.cpp)
ENDIF(APPLE)

IF(BUILD_QT5_SUPPORT_LIB)
	add_subdirectory(QT)
ENDIF()

IF(BUILD_OPENGL_LIB)
    SET(PLATFORM_GLFW_SOURCE    GLFW/PlatformGLFW.cpp
                                GLFW/WindowGLFW.cpp)

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

        SOURCE_GROUP("GLFW3"   FILES ${GLFW_SOURCE})

        add_library(CM.Platform STATIC ${PLATFORM_GLFW_SOURCE} ${GLFW_SOURCE} Window.cpp)
    ELSE(WIN32)
        add_library(CM.Platform STATIC ${PLATFORM_GLFW_SOURCE} Window.cpp)
    ENDIF(WIN32)

ENDIF()
