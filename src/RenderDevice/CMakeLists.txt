set(CM_API_ROOT_SOURCE              API/OpenGLAPI.cpp)

set(CM_API_TEXTURE_SOURCE           API/apiTexture.cpp
                                    API/apiTextureBind.cpp
                                    API/apiTextureBindStorage.cpp
                                    API/apiTextureDSA.cpp)

set(CM_API_VBO_SOURCE               API/apiVertexBufferObject.cpp
                                    API/apiVertexBufferObjectBind.cpp
                                    API/apiVertexBufferObjectBindStorage.cpp
                                    API/apiVertexBufferObjectDSA.cpp)

set(CM_API_UBO_SOURCE               API/apiUniformBufferObject.cpp
                                    API/apiUniformBufferObjectBind.cpp
                                    API/apiUniformBufferObjectBindStorage.cpp
                                    API/apiUniformBufferObjectDSA.cpp)

set(CM_API_RENDERABLE_SOURCE        API/apiRenderable.cpp
                                    API/apiRenderableVBO.cpp
                                    API/apiRenderableVBOVAO.cpp)

set(CM_API_DRAW_SOURCE              API/apiDraw.cpp
                                    API/apiDrawMulti.cpp
                                    API/apiDrawInstance.cpp
                                    API/apiDrawIndirect.cpp)

SOURCE_GROUP("OpenGL\\API"                      FILES ${CM_API_ROOT_SOURCE})
SOURCE_GROUP("OpenGL\\API\\Texture"             FILES ${CM_API_TEXTURE_SOURCE})
SOURCE_GROUP("OpenGL\\API\\VertexBufferObject"  FILES ${CM_API_VBO_SOURCE})
SOURCE_GROUP("OpenGL\\API\\UniformBufferObject" FILES ${CM_API_UBO_SOURCE})
SOURCE_GROUP("OpenGL\\API\\Renderable"          FILES ${CM_API_RENDERABLE_SOURCE})
SOURCE_GROUP("OpenGL\\API\\Draw"                FILES ${CM_API_DRAW_SOURCE})

set(CM_OPENGL_API_SOURCE            ${CM_API_ROOT_SOURCE}
                                    ${CM_API_TEXTURE_SOURCE}
                                    ${CM_API_VBO_SOURCE}
                                    ${CM_API_UBO_SOURCE}
                                    ${CM_API_RENDERABLE_SOURCE}
                                    ${CM_API_DRAW_SOURCE})

set(CM_OPENGL_SOURCE                glDebug.cpp
                                    glBuffer.cpp
                                    glCaps.cpp
                                    glFace.cpp
                                    glTexture.cpp
                                    glViewport.cpp)

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

set(CM_RENDER_SOURCE    # 			Debugger_ATI_GPU_Perf.cpp
                                    Render.cpp
                                    RenderBuffer.cpp
                                    TextureChannels.cpp
                                    TextureFormat.cpp
                        # 			ScreenShot.cpp
                                    )

set(CM_RENDER_DEVICE_SOURCE         ${CM_PLATFORM_BASE_SOURCE}
                                    ${CM_OPENGL_API_SOURCE}
									${CM_OPENGL_SOURCE}
                                    ${CM_OPENGL_CORE_45_SOURCE}
                                    ${CM_GLSL_SHADER_MAKER_SOURCE}
                                    ${CM_GLSL_SOURCE}
                                    ${CM_RENDER_SOURCE})

SET(GLEW_SOURCE                     GLEW/glew.c)

SOURCE_GROUP("OpenGL\\GLEW"                 FILES ${GLEW_SOURCE})

SOURCE_GROUP("OpenGL"						FILES ${CM_OPENGL_SOURCE})
SOURCE_GROUP("OpenGL\\Core45"				FILES ${CM_OPENGL_CORE_45_SOURCE})
SOURCE_GROUP("OpenGL\\GLSL"					FILES ${CM_GLSL_SOURCE})
SOURCE_GROUP("OpenGL\\GLSL\\ShaderMaker"	FILES ${CM_GLSL_SHADER_MAKER_SOURCE})

IF(WIN32)
	add_library(CM.RenderDevice STATIC ${CM_RENDER_DEVICE_SOURCE} ${GLEW_SOURCE})
ELSE()
	add_library(CM.RenderDevice SHARED ${CM_RENDER_DEVICE_SOURCE} ${GLEW_SOURCE})
ENDIF()
