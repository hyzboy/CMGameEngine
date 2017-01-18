#include<hgl/type/BaseString.h>
#include<hgl/LogInfo.h>
#include<glew/include/GL/glew.h>

namespace hgl
{
    namespace graph
    {
        namespace opengl_debug_message
        {
            const char *source_name[]=
            {
                "API",
                "Windows System",
                "Shader Compiler",
                "Third Party",
                "Application",
                "Other"
            };

            const char *type_name[]=
            {
                "error",
                "Deprecated Behavior",
                "Undefined Behavior",
                "Portability",
                "Performance",
                "Other",
                "Marker",
                "Push Group",
                "Pop Group",
            };

            struct severity_info
            {
                GLenum severity;
                char str[32];
            };

            const severity_info severity_name[]=
            {
                {GL_DEBUG_SEVERITY_NOTIFICATION ,"notification"},
                {GL_DEBUG_SEVERITY_HIGH         ,"high"},
                {GL_DEBUG_SEVERITY_MEDIUM       ,"medium"},
                {GL_DEBUG_SEVERITY_LOW          ,"low"},
                {0,"unkown"}
            };

            void GLAPIENTRY OpenGLDebugProc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void * /*userParam*/)
            {
                UTF8String severity_string;

                for(int i=0;;i++)
                {
                    if(severity_name[i].severity==severity
                    ||severity_name[i].severity==0)
                    {
                        severity_string=severity_name[i].str;
                        break;
                    }
                }

                LOG_INFO(U8_TEXT("OpenGL DebugMessage: source[")
                        +UTF8String(source_name[source-GL_DEBUG_SOURCE_API_ARB])
                        +U8_TEXT("] type[")
                        +UTF8String(type_name[type-GL_DEBUG_TYPE_ERROR_ARB])
                        +U8_TEXT("] severity[")
                        +severity_string
                        +U8_TEXT("] message:")+UTF8String(message,length));
            }
        }//opengl_debug_message

        namespace opengl_debug_message_amd
        {
            const char *category_name[]=
            {
                "API",
                "Windows System",
                "Deprecation",
                "Undefined Behavior",
                "Performance",
                "Shader Compiler",
                "Application",
                "Other"
            };

            const char *severity_name[]=
            {
                "high",
                "medium",
                "low"
            };

            void GLAPIENTRY OpenGLDebugProc(GLuint id, GLenum category, GLenum severity, GLsizei length, const GLchar* message, void * /*userParam*/)
            {
                LOG_INFO(U8_TEXT("OpenGL DebugMessage: id[")
                        +UTF8String(id)
                        +U8_TEXT("] category[")
                        +UTF8String(category_name[category-GL_DEBUG_CATEGORY_API_ERROR_AMD])
                        +U8_TEXT("] severity[")
                        +UTF8String(severity_name[severity-GL_DEBUG_SEVERITY_HIGH_AMD])
                        +U8_TEXT("] message:")+UTF8String(message,length));
            }
        }//namespace opengl_debug_message_amd

        void InitOpenGLDebug()
        {
            if(GLEW_VERSION_4_3||GLEW_KHR_debug)
            {
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

                glDebugMessageCallback(opengl_debug_message::OpenGLDebugProc,nullptr);

                glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DEBUG_SEVERITY_NOTIFICATION, 0,nullptr,GL_FALSE);
            }
            else
            if(GLEW_ARB_debug_output)
            {
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

                glDebugMessageCallbackARB(opengl_debug_message::OpenGLDebugProc,nullptr);

                glDebugMessageControlARB(GL_DEBUG_SOURCE_API_ARB, GL_DEBUG_TYPE_OTHER_ARB, GL_DEBUG_SEVERITY_NOTIFICATION, 0,nullptr,GL_FALSE);
            }
            else
            if(GLEW_AMD_debug_output)
            {
                glDebugMessageCallbackAMD(opengl_debug_message_amd::OpenGLDebugProc,nullptr);

                glDebugMessageEnableAMD(GL_DEBUG_CATEGORY_API_ERROR_AMD, GL_DEBUG_LOGGED_MESSAGES_AMD, 0,nullptr,GL_FALSE);
            }
        }
    }//namespace graph
}//namespace hgl
