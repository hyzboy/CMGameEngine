#include<hgl/platform/Window.h>

namespace hgl
{
    namespace platform
    {
        /**
        * 应用程序窗口基类
        */
        Window::Window()
        {
            caption=OS_TEXT("CMGDK Window");

            hglSetPropertyRead(FullScreen,	this,Window::GetFullScreen);
            hglSetPropertyRead(MouseCoord,	this,Window::GetMouseCoord);

            hglSetPropertyRead(Width,		this,Window::GetWidth);
            hglSetPropertyRead(Height,		this,Window::GetHeight);

            width=0;
            height=0;

            full_screen=false;

            mouse_coord.Zero();
        }

        Window::~Window()
        {
        }
    }//namespace platform
}//namespace hgl
