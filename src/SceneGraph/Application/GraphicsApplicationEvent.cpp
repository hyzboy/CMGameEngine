#include<hgl/platform/GraphicsApplication.h>
#include<glfw3.h>

namespace hgl
{
	namespace graph
	{
		extern GraphicsApplication *graphics_application;

		namespace
		{
			void ProcGLFWwindowSize(GLFWwindow *win,int width,int height)
			{
				graphics_application->ProcResize(width,height);
			}

			void ProcGLFWwindowClose(GLFWwindow *win)
			{
				graphics_application->ProcClose();
			}

			void ProcGLFWwindowCursorPos(GLFWwindow *win,double x,double y)
			{
				graphics_application->Proc_CursorPos(x,y);
			}

			void ProcGLFWwindowScroll(GLFWwindow *win,double x,double y)
			{
				graphics_application->Proc_Scroll(x,y);
			}

			void ProcGLFWwindowMouseButton(GLFWwindow *win,int button,int action,int mods)
			{
				graphics_application->Proc_MouseButton(button,action&GLFW_PRESS);
			}

			void ProcGLFWwindowKey(GLFWwindow *win,int key,int scancode,int action,int mods)
			{
				graphics_application->Proc_Key(key,action&GLFW_PRESS);
			}

			void ProcGLFWwindowChar(GLFWwindow *win,unsigned int character)
			{
				graphics_application->Proc_Char(character);
			}
		}

		void GraphicsApplication::InitProcEvent()
		{
			glfwSetWindowSizeCallback(glfw_win,ProcGLFWwindowSize);
			glfwSetWindowCloseCallback(glfw_win,ProcGLFWwindowClose);

			glfwSetCursorPosCallback(glfw_win,ProcGLFWwindowCursorPos);
			glfwSetScrollCallback(glfw_win,ProcGLFWwindowScroll);
			glfwSetMouseButtonCallback(glfw_win,ProcGLFWwindowMouseButton);
			glfwSetKeyCallback(glfw_win,ProcGLFWwindowKey);
			glfwSetCharCallback(glfw_win,ProcGLFWwindowChar);
		}
	}//namespace graph
}//namespace hgl
