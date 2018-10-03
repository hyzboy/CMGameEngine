SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)

OPTION(RENDER_SUPPORT_SOFTWARE  "Support Software Render"               ON)
OPTION(RENDER_SUPPORT_GLCORE    "Support OpenGL Core profile Render"    OFF)
OPTION(RENDER_SUPPORT_GLES      "Support OpenGL ES2/3 profile Render"   OFF)
OPTION(RENDER_SUPPORT_VULKAN    "Support Vulkan Render"                 OFF)
# OPTION(RENDER_SUPPORT_ORBIS     "Support Orbis Render"                  OFF)

IF(RENDER_SUPPORT_GLCORE OR RENDER_SUPPORT_GLES OR RENDER_SUPPORT_VULKAN)
    SET(USE_OPENGL ON)
ELSE()
    SET(USE_OPENGL OFF)
ENDIF()

IF(USE_OPENGL)

    IF(UNIX)
        OPTION(OPENGL_USE_EGL         "Use EGL"           OFF)
        OPTION(OPENGL_USE_EGL_WAYLAND "Use Wayland-EGL"   OFF)

        IF(OPENGL_USE_EGL OR OPENGL_USE_EGL_WAYLAND)
            add_definitions("-DUSE_EGL")

            SET(OPENGL_USE_GLFW OFF)
        ELSE()
            add_definitions("-DGLEW_STATIC")

            SET(OPENGL_USE_GLFW ON)
        ENDIF()

	ENDIF()

	IF(RENDER_SUPPORT_GLCORE)
		ADD_DEFINITIONS("-D_GLFW_USE_OPENGL")

		message("Use OpenGL Core")

		find_package(OpenGL REQUIRED)

		IF(UNIX)
			SET(HGL_OpenGL_LIB GL)
		ELSE()
			SET(HGL_OpenGL_LIB OpenGL32)
		ENDIF()
#     ELSEIF(RENDER_SUPPORT_GLES)
# # 		ADD_DEFINITIONS("-DGLFW_INCLUDE_ES2")
#         ADD_DEFINITIONS("-D_GLFW_USE_GLESV2")
#         message("Use OpenGL ES2")
#         SET(HGL_OpenGL_LIB GLESv2)
#         find_package(GLESv2 REQUIRED)
    ELSEIF(RENDER_SUPPORT_GLES)
        ADD_DEFINITIONS("-D_GLFW_USE_GLESV3")
        message("Use OpenGL ES3")
        find_package(GLESv3 REQUIRED)
    ENDIF()

	IF(OPENGL_USE_EGL)
		IF(OPENGL_USE_EGL_WAYLAND)
			SET(HGL_OpenGL_LIB ${HGL_OpenGL_LIB} wayland-egl)
		ELSE()
			SET(HGL_OpenGL_LIB ${HGL_OpenGL_LIB} egl)
		ENDIF()
	ENDIF(OPENGL_USE_EGL)

	IF(OPENGL_USE_GLFW)
        SET(HGL_OpenGL_LIB ${HGL_OpenGL_LIB} glfw)
	ENDIF()
ENDIF(USE_OPENGL)
