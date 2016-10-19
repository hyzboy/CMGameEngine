#include"PlatformGLFW.h"
#include<hgl/platform/Window.h>

namespace hgl
{
    namespace platform
    {
        class WindowGLFW:public Window
        {
            GLFWwindow *glfw_win;

        public:

            WindowGLFW(GLFWwindow *w,bool fs)
            {
                glfw_win=w;
                this->full_screen=fs;
            }

            ~WindowGLFW()override{glfwDestroyWindow(glfw_win);}

            void ToMin()override{glfwIconifyWindow(glfw_win);}
            void ToMax()override{glfwMaximizeWindow(glfw_win);}

            void Show()override{glfwShowWindow(glfw_win);}
            void Hide()override{glfwHideWindow(glfw_win);}

            void SetCaption(const OSString &name)override
            {
                glfwSetWindowTitle(glfw_win,name);
                this->caption=name;
            }

            void SetViewport(int l,int t,int w,int h)override
            {
            }

            void SetMouseCoord(const Vertex2i &coord)override
            {
                glfwSetCursorPos(glfw_win,coord.x,coord.y);
                mouse_coord=coord;
            }

            void SetSize(int w,int h)override{glfwSetWindowSize(glfw_win,w,h);}

            void MakeToCurrent()override{glfwMakeContextCurrent(glfw_win);}

            void SwapBuffer()override{glfwSwapBuffers(glfw_win);}

            void WaitEvent(const double &time_out)override
            {
                if(time_out>0)
                    glfwWaitEventsTimeout(time_out);
                else
                    glfwWaitEvents();
            }

            void PollEvent()override{glfwPollEvents();}
        };//class WindowGLFW

        void SetGLFWWindowHint(const OpenGLSetup *gs)
        {
			glfwWindowHint(GLFW_SAMPLES,				gs->MultiSample);

			glfwWindowHint(GLFW_CLIENT_API,				gs->opengl_es?GLFW_OPENGL_ES_API:GLFW_OPENGL_API);

            glfwWindowHint(GLFW_CONTEXT_CREATION_API,   gs->egl?GLFW_NATIVE_CONTEXT_API:GLFW_EGL_CONTEXT_API);

            if(gs->opengl_es)
            {
            }
            else
            {
                glfwWindowHint(GLFW_OPENGL_PROFILE,			GLFW_OPENGL_CORE_PROFILE);		//核心模式
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,	true);							//向前兼容模式(无旧特性)
            }

			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,	gs->debug);                        //调试模式
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,	gs->major);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,	gs->minor);

			glfwWindowHint(GLFW_VISIBLE,				true);								//是否显示

			glfwWindowHint(GLFW_ALPHA_BITS,          gs->AlphaBits	);
			glfwWindowHint(GLFW_DEPTH_BITS,          gs->DepthBits	);
			glfwWindowHint(GLFW_STENCIL_BITS,        gs->StencilBits);

            glfwWindowHint(GLFW_ACCUM_RED_BITS,     gs->AccumRedBits    );
            glfwWindowHint(GLFW_ACCUM_GREEN_BITS,   gs->AccumGreenBits  );
            glfwWindowHint(GLFW_ACCUM_BLUE_BITS,    gs->AccumBlueBits   );
            glfwWindowHint(GLFW_ACCUM_ALPHA_BITS,   gs->AccumAlphaBits  );
        }

        Window *Create(int width,int height,const WindowSetup *ws,const OpenGLSetup *gs)
        {
            SetGLFWWindowHint(gs);

            glfwWindowHint(GLFW_MAXIMIZED,ws->Maximize);
            glfwWindowHint(GLFW_RESIZABLE,ws->Resize);

			GLFWwindow *win=glfwCreateWindow(	width,
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
			glfwWindowHint(GLFW_REFRESH_RATE,	vm->freq);

			GLFWwindow *win=glfwCreateWindow(	vm->width,
                                                vm->height,
                                                "CMGDK Window",
                                                ((MonitorGLFW *)monitor)->glfw_monitor,
                                                nullptr);

            if(!win)RETURN_ERROR_NULL;

            return(new WindowGLFW(win,true));
        }
    }//namespace platform
}//namespace hgl
