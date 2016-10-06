set(CM_OPENGL_SOURCE                glBuffer.cpp
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

set(CM_RENDER_DEVICE_SOURCE         ${CM_OPENGL_SOURCE}
                                    ${CM_OPENGL_CORE_45_SOURCE}
                                    ${CM_GLSL_SHADER_MAKER_SOURCE}
                                    ${CM_GLSL_SOURCE}
                                    ${CM_RENDER_SOURCE})

SOURCE_GROUP("OpenGL"                     FILES ${CM_OPENGL_SOURCE})
SOURCE_GROUP("OpenGL\\Core45"             FILES ${CM_OPENGL_CORE_45_SOURCE})
SOURCE_GROUP("OpenGL\\GLSL"               FILES ${CM_GLSL_SOURCE})
SOURCE_GROUP("OpenGL\\GLSL\\ShaderMaker"  FILES ${CM_GLSL_SHADER_MAKER_SOURCE})

IF(WIN32)

      add_definitions("-D_GLFW_WIN32")
      add_definitions("-D_GLFW_WGL")

      set(GLFW_SOURCE		GLFW/context.c
                            GLFW/egl_context.c
                            GLFW/init.c
                            GLFW/input.c
                            GLFW/monitor.c
                            GLFW/vulkan.c
                            GLFW/wgl_context.c
                            GLFW/win32_init.c
                            GLFW/win32_joystick.c
                            GLFW/win32_monitor.c
                            GLFW/win32_time.c
                            GLFW/win32_tls.c
                            GLFW/win32_window.c
                            GLFW/window.c)
                            
      SOURCE_GROUP("OpenGL\\GLFW"   FILES ${GLFW_SOURCE})
      
      SET(GLEW_SOURCE       GLEW/glew.c)
      
      SOURCE_GROUP("OpenGL\\GLEW"   FILES ${GLEW_SOURCE})

	add_library(CM.RenderDevice STATIC ${CM_RENDER_DEVICE_SOURCE} ${GLFW_SOURCE} ${GLEW_SOURCE})
ELSE()
	add_library(CM.RenderDevice SHARED ${CM_RENDER_DEVICE_SOURCE})
ENDIF()
