add_definitions("-DGLEW_STATIC")

add_library(RenderDevice SHARED
			${GLEW_SOURCE}
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
