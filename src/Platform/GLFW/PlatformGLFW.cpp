#include"PlatformGLFW.h"
#include<GLFW/glfw3.h>

namespace hgl
{
	void InitKeyConvert();

    namespace platform
    {
        namespace
        {
            static UTF8String GLFW_INIT_ERROR_CAPTION="Init GLFW Error";

            static bool glfw_init=false;
            static bool glfw_init_result=false;

            static MonitorGLFW *default_monitor=nullptr;
            static ObjectList<Monitor> MonitorList;
        }//namespace

        const bool InitGraphicsPlatform()
        {
            InitKeyConvert();

            glfw_init=true;

            glfw_init_result=glfwInit();

            return glfw_init_result;
        }

        void CloseGraphicsPlatform()
        {
            if(glfw_init_result)
            {
                glfwTerminate();

                glfw_init_result=false;
                glfw_init=false;
            }
        }

        const bool CheckGLFWInit()
        {
            if(!glfw_init)
                InitGraphicsPlatform();

            return glfw_init_result;
        }

        const UTF8String &GetPlatformName()
        {
            if(!CheckGLFWInit())
                return GLFW_INIT_ERROR_CAPTION;

            static UTF8String platform_name;

            if(platform_name.IsEmpty())
            {
                int major,minor,rev;

                glfwGetVersion(&major,&minor,&rev);

                platform_name=U8_TEXT("GLFW ")+UTF8String(major)+ U8_TEXT(".")+ UTF8String(minor)+ U8_TEXT(".")+ UTF8String(rev);
            }

            return platform_name;
        }

        MonitorGLFW *GetMonitorAttrib(GLFWmonitor *gm)
        {
            MonitorGLFW *m=new MonitorGLFW;

            m->glfw_monitor=gm;
            m->name=glfwGetMonitorName(gm);
            glfwGetMonitorPhysicalSize(gm,&(m->width),&(m->height));
            glfwGetMonitorPos(gm,&(m->x),&(m->y));

            m->cur_video_mode=nullptr;

            return m;
        }

        const Monitor *GetDefaultMonitor()
        {
            if(!CheckGLFWInit())RETURN_ERROR_NULL;

            if(!default_monitor)
                default_monitor=GetMonitorAttrib(glfwGetPrimaryMonitor());

            return default_monitor;
        }

        const ObjectList<Monitor> &GetMonitorList()
        {
            if(MonitorList.GetCount()<=0)
            {
                int count=0;

                GLFWmonitor **ml=glfwGetMonitors(&count);

                for(int i=0;i<count;i++)
                    MonitorList+=GetMonitorAttrib(ml[i]);
            }

            return MonitorList;
        }

        VideoMode *ConvertVideoMode(const GLFWvidmode *mode)
        {
            if(mode)
                return nullptr;

            VideoMode *vm=new VideoMode;

            vm->width   =mode->width;
            vm->height  =mode->height;
            vm->freq    =mode->refreshRate;

            vm->red     =mode->redBits;
            vm->green   =mode->greenBits;
            vm->blue    =mode->blueBits;

            vm->bit     =vm->red+vm->green+vm->blue;

            return vm;
        }

        const VideoMode *GetCurVideoMode(const Monitor *m)
        {
            if(!CheckGLFWInit())RETURN_ERROR_NULL;

            MonitorGLFW *monitor;

            if(m)
                monitor=(MonitorGLFW *)m;
            else
                monitor=(MonitorGLFW *)GetDefaultMonitor();

            if(!monitor->cur_video_mode)
            {
                const GLFWvidmode *mode=glfwGetVideoMode(monitor->glfw_monitor);

                if(!mode)
                    RETURN_ERROR_NULL;

                monitor->cur_video_mode=ConvertVideoMode(mode);
            }

            return monitor->cur_video_mode;
        }

        const ObjectList<VideoMode> &GetVideoMode(const Monitor *m)
        {
            MonitorGLFW *monitor;

            if(m)
                monitor=(MonitorGLFW *)m;
            else
                monitor=(MonitorGLFW *)GetDefaultMonitor();

            if(monitor->video_mode_list.GetCount()>0)
                return monitor->video_mode_list;

            int count=0;

            const GLFWvidmode *ml=glfwGetVideoModes(monitor->glfw_monitor,&count);

            for(int i=0;i<count;i++)
            {
                VideoMode *vm=ConvertVideoMode(ml+i);

                if(ml)
                    monitor->video_mode_list.Add(vm);
            }

            return monitor->video_mode_list;
        }
    }//namespace platform
}//namespace hgl
