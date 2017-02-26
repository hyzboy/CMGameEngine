#include"WindowGLFW.h"
#include"PlatformGLFW.h"
#include<hgl/Info.h>
#include<hgl/platform/PlatformInterface.h>
#include<GLFW/glfw3.h>

namespace hgl
{
    namespace platform
    {
        WindowGLFW::WindowGLFW(GLFWwindow *w,bool fs)
        {
            glfw_win=w;
            this->full_screen=fs;
        }

        WindowGLFW::~WindowGLFW()
        {
            glfwDestroyWindow(glfw_win);
        }

        void WindowGLFW::ToMin(){glfwIconifyWindow(glfw_win);}
        void WindowGLFW::ToMax(){glfwMaximizeWindow(glfw_win);}

        void WindowGLFW::Show(){glfwShowWindow(glfw_win);}
        void WindowGLFW::Hide(){glfwHideWindow(glfw_win);}

        void WindowGLFW::SetCaption(const OSString &name)
        {
#if HGL_OS == HGL_OS_Windows
            glfwSetWindowTitle(glfw_win,to_u8(name));
#else
            glfwSetWindowTitle(glfw_win,name);
#endif//
            this->caption=name;
        }

        void WindowGLFW::SetMouseCoord(const Vertex2i &coord)
        {
            glfwSetCursorPos(glfw_win,coord.x,coord.y);
            mouse_coord=coord;
        }

        void WindowGLFW::SetSize(int w,int h)
        {
            glfwSetWindowSize(glfw_win,w,h);
            this->width=w;
            this->height=h;
        }

        void WindowGLFW::MakeToCurrent(){glfwMakeContextCurrent(glfw_win);}

        void WindowGLFW::SwapBuffer(){glfwSwapBuffers(glfw_win);}

        void WindowGLFW::WaitEvent(const double &time_out)
        {
            if(time_out>0)
                glfwWaitEventsTimeout(time_out);
            else
                glfwWaitEvents();
        }

        void WindowGLFW::PollEvent(){glfwPollEvents();}

        bool WindowGLFW::IsOpen(){return(!glfwWindowShouldClose(glfw_win));}
    }//namespace platform

    namespace platform
    {
        void SetGLFWWindowHint(const OpenGLSetup *gs)
        {
            glfwWindowHint(GLFW_SAMPLES,                gs->MultiSample);

            glfwWindowHint(GLFW_CLIENT_API,             gs->es?GLFW_OPENGL_ES_API:GLFW_OPENGL_API);

            glfwWindowHint(GLFW_CONTEXT_CREATION_API,   gs->egl?GLFW_EGL_CONTEXT_API:GLFW_NATIVE_CONTEXT_API);

            if(gs->es)
            {
            }
            else
            {
                glfwWindowHint(GLFW_OPENGL_PROFILE,         GLFW_OPENGL_CORE_PROFILE);        //核心模式
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,  true);                            //向前兼容模式(无旧特性)
            }

            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,       gs->debug);                       //调试模式
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,      gs->major);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,      gs->minor);

            glfwWindowHint(GLFW_VISIBLE,                    true);                            //是否显示

            if(gs->no_use_stencil   )glfwWindowHint(GLFW_STENCIL_BITS,  0); else
            if(gs->StencilBits  >0  )glfwWindowHint(GLFW_STENCIL_BITS,  gs->StencilBits);
            if(gs->AlphaBits    >0  )glfwWindowHint(GLFW_ALPHA_BITS,    gs->AlphaBits   );
            if(gs->DepthBits    >0  )glfwWindowHint(GLFW_DEPTH_BITS,    gs->DepthBits   );

            if(gs->AccumRedBits     >0)glfwWindowHint(GLFW_ACCUM_RED_BITS,     gs->AccumRedBits    );
            if(gs->AccumGreenBits   >0)glfwWindowHint(GLFW_ACCUM_GREEN_BITS,   gs->AccumGreenBits  );
            if(gs->AccumBlueBits    >0)glfwWindowHint(GLFW_ACCUM_BLUE_BITS,    gs->AccumBlueBits   );
            if(gs->AccumAlphaBits   >0)glfwWindowHint(GLFW_ACCUM_ALPHA_BITS,   gs->AccumAlphaBits  );
        }

        Window *Create(int width,int height,const WindowSetup *ws,const OpenGLSetup *gs)
        {
            SetGLFWWindowHint(gs);

            glfwWindowHint(GLFW_MAXIMIZED,ws->Maximize);
            glfwWindowHint(GLFW_RESIZABLE,ws->Resize);

            GLFWwindow *win=glfwCreateWindow(    width,
                                                height,
                                                ws->Name,
                                                nullptr,
                                                nullptr);

            if(!win)RETURN_ERROR_NULL;

            return(new WindowGLFW(win,false));
        }

        Window *Create(const Monitor *monitor,const VideoMode *vm,const OpenGLSetup *gs)
        {
            SetGLFWWindowHint(gs);

            glfwWindowHint(GLFW_RED_BITS,       vm->red);
            glfwWindowHint(GLFW_GREEN_BITS,     vm->green);
            glfwWindowHint(GLFW_BLUE_BITS,      vm->blue);
            glfwWindowHint(GLFW_REFRESH_RATE,   vm->freq);

            GLFWwindow *win=glfwCreateWindow(   vm->width,
                                                vm->height,
                                                "CMGDK Window",
                                                monitor?((MonitorGLFW *)monitor)->glfw_monitor:nullptr,
                                                nullptr);

            if(!win)RETURN_ERROR_NULL;

            return(new WindowGLFW(win,true));
        }
    }//namespace platform
}//namespace hgl
