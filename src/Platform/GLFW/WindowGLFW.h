#include<hgl/platform/Window.h>

typedef struct GLFWwindow GLFWwindow;

namespace hgl
{
    namespace platform
    {
        class WindowGLFW:public Window
        {
            GLFWwindow *glfw_win;

        public:

            WindowGLFW(GLFWwindow *w,bool fs);
            ~WindowGLFW()override;

            void ToMin()override;
            void ToMax()override;

            void Show()override;
            void Hide()override;

            void SetCaption(const OSString &name)override;
            void SetMouseCoord(const Vertex2i &coord)override;
            void SetSize(int w,int h)override;

            void MakeToCurrent()override;
            void SwapBuffer()override;
            void WaitEvent(const double &time_out)override;
            void PollEvent()override;
            bool IsOpen()override;
            void InitProcEvent(RootFlowControl *)override;
        };//class WindowGLFW
    }//namespace platform
}//namespace hgl
