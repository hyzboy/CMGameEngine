#ifndef HGL_PLATFORM_GLFW_INCLUDE
#define HGL_PLATFORM_GLFW_INCLUDE

#include<hgl/platform/PlatformInterface.h>
#include<GLFW/glfw3.h>
#include<hgl/type/List.h>

namespace hgl
{
    namespace platform
    {
        struct MonitorGLFW:public Monitor
        {
            GLFWmonitor *glfw_monitor;

            VideoMode *cur_video_mode;
            ObjectList<VideoMode> video_mode_list;
        };
    }//namespace platform
}//namespace hgl
#endif//HGL_PLATFORM_GLFW_INCLUDE
