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
                GraphicsApplication *app=(GraphicsApplication *)glfwGetWindowUserPointer(win);

                if(app)
                    app->ProcResize(width,height);
			}

			void ProcGLFWwindowClose(GLFWwindow *win)
			{
                GraphicsApplication *app=(GraphicsApplication *)glfwGetWindowUserPointer(win);

                if(app)
                    app->ProcClose();
			}

			void ProcGLFWwindowCursorPos(GLFWwindow *win,double x,double y)
			{
                GraphicsApplication *app=(GraphicsApplication *)glfwGetWindowUserPointer(win);

                if(app)
                    app->Proc_CursorPos(x,y);
			}

			void ProcGLFWwindowScroll(GLFWwindow *win,double x,double y)
			{
                GraphicsApplication *app=(GraphicsApplication *)glfwGetWindowUserPointer(win);

                if(app)
                    app->Proc_Scroll(x,y);
			}

			void ProcGLFWwindowMouseButton(GLFWwindow *win,int button,int action,int mods)
			{
                GraphicsApplication *app=(GraphicsApplication *)glfwGetWindowUserPointer(win);

                if(app)
                    app->Proc_MouseButton(button,action&GLFW_PRESS);
			}

			void ProcGLFWwindowKey(GLFWwindow *win,int key,int scancode,int action,int mods)
			{
                GraphicsApplication *app=(GraphicsApplication *)glfwGetWindowUserPointer(win);

                if(app)
                    app->Proc_Key(key,action&GLFW_PRESS);
			}

			void ProcGLFWwindowChar(GLFWwindow *win,unsigned int character)
			{
                GraphicsApplication *app=(GraphicsApplication *)glfwGetWindowUserPointer(win);

                if(app)
                    app->Proc_Char(character);
			}
		}
/*
        void JoinWindowEvent(graph::GraphicsApplication *app,Window *win)
		{
            GLFWwindow *glfw_win=((WindowGLFW *)win)->gl
            glfwSetWindowUserPointer(glfw_win,app);

			glfwSetWindowSizeCallback(glfw_win,ProcGLFWwindowSize);
			glfwSetWindowCloseCallback(glfw_win,ProcGLFWwindowClose);

			glfwSetCursorPosCallback(glfw_win,ProcGLFWwindowCursorPos);
			glfwSetScrollCallback(glfw_win,ProcGLFWwindowScroll);
			glfwSetMouseButtonCallback(glfw_win,ProcGLFWwindowMouseButton);
			glfwSetKeyCallback(glfw_win,ProcGLFWwindowKey);
			glfwSetCharCallback(glfw_win,ProcGLFWwindowChar);
		}

		void UnjonGLFWWindowEvent(GLFWwindow *glfw_win);
        {
            glfw_win_map.DeleteByIndex(glfw_win);
        }*/
    }//namespace platform
}//namespace hgl
