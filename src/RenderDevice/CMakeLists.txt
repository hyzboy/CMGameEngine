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
			ShaderLight.cpp
			UBO.cpp
			${GLEW_SOURCE})

IF(WIN32)
	add_library(CM.RenderDevice STATIC ${CM_RENDER_DEVICE_SOURCE})
ELSE()
	add_library(CM.RenderDevice SHARED ${CM_RENDER_DEVICE_SOURCE})
ENDIF()
