#include<hgl/platform/Window.h>

namespace hgl
{
	/**
	* 应用程序窗口基类
	*/
	Window::Window()
	{
		caption=OS_TEXT("CMGDK Window");

		hglSetPropertyRead(FullScreen,	this,Window::GetFullScreen);
		hglSetPropertyRead(MouseX,		this,Window::GetMouseX);
        hglSetPropertyRead(MouseY,		this,Window::GetMouseY);

		hglSetPropertyRead(Width,		this,Window::GetWidth);
		hglSetPropertyRead(Height,		this,Window::GetHeight);

		width=0;
		height=0;

		full_screen=false;
	}

	Window::~Window()
	{
	}
	
	void Window::SetSize(int w,int h)
	{
		width=w;
		height=h;
	}
}//namespace hgl
