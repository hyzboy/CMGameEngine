#ifndef WinWindowH
#define WinWindowH

#include<wchar.h>
#include<windows.h>
#include<hgl/platform/Window.h>
namespace hgl
{
	class FlowObject;

	class WinWindow:public Window
	{
	protected:

		UTF16String class_name;
		UTF16String icon_name;
		UTF16String cursor_name;

		bool edge,resize,min,max,topmost,sysmenu,right,apptaskbar;
		uint32 Style,ExStyle;
		int win_width,win_height;
		int win_left,win_top;

	protected:

		bool Registry();
		bool Create();

		void GetCaption();
		void SetCaption();

		void SetSystemCursor(bool);

		bool CreateToWindow(int,int);

	public:

		HINSTANCE hInstance;
		HWND hWnd;
		HDC hDC;

		MSG msg;

		DefEvent(bool,OnWinMessage,(HWND,UINT,WPARAM,LPARAM));										///<Windows消息处理事件

	public:

		WinWindow();
		virtual ~WinWindow();

		bool InitToFullScreen(int,int,bool);
		bool InitToWindow(int,int,GraphicsSystemInitInfo::WindowSetup &);

		void Close();

		bool ToMinWindow();
		bool ToMaxWindow();

		void Show();
		void Hide();

		virtual void SetViewport(int,int,int,int)=0;												///<设定可视范围
	};//class WinWindow
}//namespace hgl
#endif//WinWindowH
