#include<hgl/platform/GraphicsApplication.h>
#include<hgl/type/Map.h>
#include"WindowGLFW.h"
#include<GLFW/glfw3.h>

namespace hgl
{
    namespace platform
    {
        namespace
        {
            void ProcGLFWwindowSize(GLFWwindow *win,int width,int height)
            {
                AppEventBase *ev=(AppEventBase *)glfwGetWindowUserPointer(win);

                if(ev)
                    ev->Resize.Proc(width,height);
            }

            void ProcGLFWwindowClose(GLFWwindow *win)
            {
                AppEventBase *ev=(AppEventBase *)glfwGetWindowUserPointer(win);

                if(ev)
                    ev->Close.Proc();
            }

            void ProcGLFWwindowCursorPos(GLFWwindow *win,double x,double y)
            {
                AppEventBase *ev=(AppEventBase *)glfwGetWindowUserPointer(win);

                if(ev)
                    ev->MouseMove.Proc(Vertex2i(x,y));
            }

            void ProcGLFWwindowScroll(GLFWwindow *win,double x,double y)
            {
                AppEventBase *ev=(AppEventBase *)glfwGetWindowUserPointer(win);

                if(ev)
                    ev->MouseWheel.Proc(x,y);
            }

            void ProcGLFWwindowMouseButton(GLFWwindow *win,int button,int action,int mods)
            {
                AppEventBase *ev=(AppEventBase *)glfwGetWindowUserPointer(win);

                if(ev)
                    ev->MouseButton.Proc(button,action&GLFW_PRESS);
            }

            void ProcGLFWwindowKey(GLFWwindow *win,int key,int scancode,int action,int mods)
            {
                AppEventBase *ev=(AppEventBase *)glfwGetWindowUserPointer(win);

                if(ev)
                    ev->Key.Proc(key,action&GLFW_PRESS);
            }

            void ProcGLFWwindowChar(GLFWwindow *win,unsigned int character)
            {
                AppEventBase *ev=(AppEventBase *)glfwGetWindowUserPointer(win);

                if(ev)
                    ev->Char.Proc((os_char)character);
            }
        }

        void WindowGLFW::InitProcEvent(AppEventBase *ev)
        {
            glfwSetWindowUserPointer(glfw_win,ev);

            glfwSetWindowSizeCallback(glfw_win,ProcGLFWwindowSize);
            glfwSetWindowCloseCallback(glfw_win,ProcGLFWwindowClose);

            glfwSetCursorPosCallback(glfw_win,ProcGLFWwindowCursorPos);
            glfwSetScrollCallback(glfw_win,ProcGLFWwindowScroll);
            glfwSetMouseButtonCallback(glfw_win,ProcGLFWwindowMouseButton);
            glfwSetKeyCallback(glfw_win,ProcGLFWwindowKey);
            glfwSetCharCallback(glfw_win,ProcGLFWwindowChar);
        }
    }//namespace platform
}//namespace hgl
