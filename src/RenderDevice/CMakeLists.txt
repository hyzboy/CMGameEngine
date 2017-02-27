set(CM_OPENGL_VBO_SOURCE            VertexBuffer.cpp
                                    VertexBufferControlBind.cpp
                                    VertexBufferControlDSA.cpp)

set(CM_OPENGL_TEXTURE_SOURCE        Texture.cpp
                                    TextureBaseControl.cpp
                                    TextureBaseControlBind.cpp
                                    TextureBaseControlDSA.cpp
                                    Texture1D.cpp
                                    Texture1DDSA.cpp
                                    Texture2D.cpp
                                    Texture2DDSA.cpp
                                    SamplerObject.cpp)

set(CM_OPENGL_RENDERABLE_SOURCE     glRenderable.cpp)    

set(CM_OPENGL_RENDER_TO_TEXTURE     glRenderToTexture.cpp)

set(CM_OPENGL_SOURCE                glDebug.cpp
                                    glBuffer.cpp
                                    glCaps.cpp
                                    glFace.cpp
                                    glTexture.cpp
                                    glViewport.cpp
                                    OpenGLCoreExtensions.c)

#set(CM_OPENGL_CORE_45_SOURCE        OpenGLCore45/Core45Texture1D.cpp
#                                    OpenGLCore45/Core45Texture1DArray.cpp
#                                    OpenGLCore45/Core45Texture2D.cpp
#                                    OpenGLCore45/Core45Texture2DArray.cpp
#                                    OpenGLCore45/Core45Texture3D.cpp
#                                    OpenGLCore45/Core45TextureCubeMap.cpp
#                                    OpenGLCore45/Core45Renderable.cpp
#                                    OpenGLCore45/Core45VertexBuffer.cpp
#                                    OpenGLCore45/Core45RenderToTexture.cpp
#                                    )

set(CM_GLSL_SHADER_MAKER_SOURCE     ShaderMaker/ShaderStringList.cpp
                                    ShaderMaker/GenVertexShader.cpp
                                    ShaderMaker/GenFragmentShader.cpp
                                    ShaderMaker/GenGeometryShader.cpp)

set(CM_GLSL_SOURCE                  GLSL/Shader.cpp
                                    GLSL/GLSL.cpp
                                    GLSL/ShaderStorage.cpp
                                    GLSL/UBO.cpp)

set(CM_RENDER_SOURCE    #             Debugger_ATI_GPU_Perf.cpp
                                    Render.cpp
                                    RenderBuffer.cpp
                                    TextureChannels.cpp
                                    TextureFormat.cpp
                        #             ScreenShot.cpp
                                    )

set(CM_RENDER_DEVICE_SOURCE         ${CM_PLATFORM_BASE_SOURCE}
                                    ${CM_OPENGL_TEXTURE_SOURCE}
                                    ${CM_OPENGL_VBO_SOURCE}
                                    ${CM_OPENGL_RENDERABLE_SOURCE}
                                    ${CM_OPENGL_RENDER_TO_TEXTURE}
                                    ${CM_OPENGL_SOURCE}
                                    ${CM_GLSL_SHADER_MAKER_SOURCE}
                                    ${CM_GLSL_SOURCE}
                                    ${CM_RENDER_SOURCE})

SET(GLEW_SOURCE                     GLEW/glew.c)

SOURCE_GROUP("OpenGL\\GLEW"                 FILES ${GLEW_SOURCE})

SOURCE_GROUP("OpenGL"                       FILES ${CM_OPENGL_SOURCE})
SOURCE_GROUP("OpenGL\\VertexBuffer"         FILES ${CM_OPENGL_VBO_SOURCE})
SOURCE_GROUP("OpenGL\\Texture"              FILES ${CM_OPENGL_TEXTURE_SOURCE})
SOURCE_GROUP("OpenGL\\Renderable"           FILES ${CM_OPENGL_RENDERABLE_SOURCE})
SOURCE_GROUP("OpenGL\\RenderToTexture"      FILES ${CM_OPENGL_RENDER_TO_TEXTURE})
SOURCE_GROUP("OpenGL\\GLSL"                 FILES ${CM_GLSL_SOURCE})
SOURCE_GROUP("OpenGL\\GLSL\\ShaderMaker"    FILES ${CM_GLSL_SHADER_MAKER_SOURCE})

IF(WIN32)
    add_library(CM.RenderDevice STATIC ${CM_RENDER_DEVICE_SOURCE} ${GLEW_SOURCE})
ELSE()
    add_library(CM.RenderDevice SHARED ${CM_RENDER_DEVICE_SOURCE} ${GLEW_SOURCE})
ENDIF()
