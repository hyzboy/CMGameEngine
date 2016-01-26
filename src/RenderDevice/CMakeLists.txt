set(CM_OPENGL_CORE_45_SOURCE        OpenGLCore45/Core45Texture1D.cpp
									OpenGLCore45/Core45Texture1DArray.cpp
                                    OpenGLCore45/Core45Texture2D.cpp
                                    OpenGLCore45/Core45Texture2DArray.cpp
                                    OpenGLCore45/Core45Texture3D.cpp
                                    OpenGLCore45/Core45TextureCubeMap.cpp
                                    OpenGLCore45/Core45Renderable.cpp
                                    OpenGLCore45/Core45VertexBuffer.cpp
                                    OpenGLCore45/Core45RenderToTexture.cpp
                                    OpenGLCoreExtensions.c)

set(CM_GLSL_SHADER_MAKER_SOURCE     ShaderMaker/ShaderStringList.cpp
                                    ShaderMaker/GenVertexShader.cpp
                                    ShaderMaker/GenFragmentShader.cpp
                                    ShaderMaker/GenGeometryShader.cpp)

set(CM_GLSL_SOURCE                  GLSL/Shader.cpp
                                    GLSL/GLSL.cpp
                                    GLSL/ShaderStorage.cpp
                                    GLSL/UBO.cpp)


set(CM_RENDER_DEVICE_SOURCE         ${CM_OPENGL_CORE_45_SOURCE}
									${CM_GLSL_SOURCE}
                                    ${CM_GLSL_SHADER_MAKER_SOURCE}
                                    ${CM_RENDER_DEVICE_OPENGL_CORE_SOURCE}
                                    ${GLEW_SOURCE}
                        # 			Debugger_ATI_GPU_Perf.cpp
                                    glBuffer.cpp
                                    glCaps.cpp
                                    glFace.cpp
                                    glTexture.cpp
                                    glViewport.cpp
                                    Render.cpp
                                    RenderBuffer.cpp
                                    TextureChannels.cpp
                                    TextureFormat.cpp
                        # 			ScreenShot.cpp
                                    )

IF(WIN32)
	add_library(CM.RenderDevice STATIC ${CM_RENDER_DEVICE_SOURCE})
ELSE()
	add_library(CM.RenderDevice SHARED ${CM_RENDER_DEVICE_SOURCE})
ENDIF()
