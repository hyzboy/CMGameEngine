#include<hgl/platform/GraphicsApplication.h>
#include<hgl/type/Map.h>
#include<hgl/object/RootFlowControl.h>
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
                RootFlowControl *rfc=(RootFlowControl *)glfwGetWindowUserPointer(win);

                if(!rfc)return;
                    
                rfc->WindowResize(width,height);
            }

            void ProcGLFWwindowClose(GLFWwindow *win)
            {
                RootFlowControl *rfc=(RootFlowControl *)glfwGetWindowUserPointer(win);

                if(!rfc)return;

                rfc->WindowClose();
            }

            void ProcGLFWwindowCursorPos(GLFWwindow *win,double x,double y)
            {
                RootFlowControl *rfc=(RootFlowControl *)glfwGetWindowUserPointer(win);

                if(!rfc)return;
                    
                rfc->MouseMove(Vertex2i(x,y));
            }

            void ProcGLFWwindowScroll(GLFWwindow *win,double x,double y)
            {
                RootFlowControl *rfc=(RootFlowControl *)glfwGetWindowUserPointer(win);

                if(!rfc)return;
                    
                rfc->MouseWheel(x,y);
            }

            void ProcGLFWwindowMouseButton(GLFWwindow *win,int button,int action,int mods)
            {
                RootFlowControl *rfc=(RootFlowControl *)glfwGetWindowUserPointer(win);

                if(!rfc)return;
                
                if(action==GLFW_PRESS)rfc->OnMouseDown(button);else
                if(action==GLFW_REPEAT)rfc->OnMouseRepeat(button);else
                if(action==GLFW_RELEASE)rfc->OnMouseUp(button);
            }

            void ProcGLFWwindowKey(GLFWwindow *win,int key,int scancode,int action,int mods)
            {
                RootFlowControl *rfc=(RootFlowControl *)glfwGetWindowUserPointer(win);

                if(!rfc)return;
                    
                if(action==GLFW_PRESS)rfc->OnKeyDown(key);else
                if(action==GLFW_REPEAT)rfc->OnKeyRepeat(key);else
                if(action==GLFW_RELEASE)rfc->OnKeyUp(key);
            }

            void ProcGLFWwindowChar(GLFWwindow *win,unsigned int character)
            {
                RootFlowControl *rfc=(RootFlowControl *)glfwGetWindowUserPointer(win);

                if(!rfc)return;
                    
                rfc->OnChar((os_char)character);
            }
        }

        void WindowGLFW::InitProcrfcent(RootFlowControl *rfc)
        {
            glfwSetWindowUserPointer(glfw_win,rfc);

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
