add_definitions("-DGLEW_STATIC")

set(CM_RENDER_DEVICE_SOURCE
# 			Debugger_ATI_GPU_Perf.cpp
			GenFragmentShader.cpp
			GenGeometryShader.cpp
			GenVertexShader.cpp
			glBuffer.cpp
			glCaps.cpp
			glFace.cpp
			GLSL.cpp
			glTexture.cpp
			glViewport.cpp
			OpenGLCoreExtensions.c
			OpenGLRenderable.cpp
			OpenGLTexture.cpp
			OpenGLVertexBuffer.cpp
			Render.cpp
# 			ScreenShot.cpp
			ShaderStringList.cpp
			UBO.cpp)

IF(WIN32)
	add_library(CM.RenderDevice STATIC ${CM_RENDER_DEVICE_SOURCE} glew.c)
ELSE()
	add_library(CM.RenderDevice SHARED ${CM_RENDER_DEVICE_SOURCE})
ENDIF()