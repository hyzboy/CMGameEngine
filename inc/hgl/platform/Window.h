#ifndef HGL_WINDOW_INCLUDE
#define HGL_WINDOW_INCLUDE

#include<hgl/platform/GraphicsSystemInitInfo.h>
namespace hgl
{
	/**
	* 应用程序窗口基类
	*/
	class Window
	{
	protected:

		OSString caption;
		bool full_screen;
        int mouse_x,mouse_y;

		int width,height;

		int GetWidth(){return width;}
		int GetHeight(){return height;}

		bool GetFullScreen(){return full_screen;}
		int GetMouseX(){return mouse_x;}
		int GetMouseY(){return mouse_y;}

	public:

		Property<bool>			FullScreen;															///<是否全屏状态
		Property<int>       	MouseX;
		Property<int>       	MouseY;
		Property<int>			Width;																///<窗口宽度
		Property<int>			Height;																///<窗口高度
		Property<bool>			SystemCursor;														///<是否使用系统光标

	public:	//方法

		Window();																					///<本类构造函数
		virtual ~Window();																			///<本类析构函数

		virtual bool InitToFullScreen(int,int,int,bool)=0;											///<初始化到全屏模式
		virtual bool InitToWindow(int,int,GraphicsSystemInitInfo::WindowSetup &)=0;					///<初始化到窗口模式

		virtual void Close()=0;																		///<关闭程序窗口

		virtual bool ToMinWindow()=0;																///<窗口最小化
		virtual bool ToMaxWindow()=0;																///<窗口最大化

		virtual void Show()=0;																		///<显示窗口
		virtual void Hide()=0;																		///<隐藏窗口

		virtual const OSString &GetCaption()const{return caption;}
		virtual void SetCaption(const OSString &cap){caption=cap;}

		virtual void SetViewport(int,int,int,int)=0;												///<设定可视范围
				void SetViewport(){SetViewport(0,0,width,height);}									///<设定可视范围为整个屏幕或窗口
				void SetViewport(int w,int h){SetViewport((width-w)/2,(height-h)/2,w,h);}			///<设定可视范围为居中显示的部分屏幕或窗口

	public:	//被实际操作系统接口层所调用的函数，在不了解的情况下请不要使用

		virtual void SetMouseCoord(int x,int y){mouse_x=x;mouse_y=y;}								///<设置鼠标坐标
		virtual void SetSize(int w,int h);															///<设置窗口大小

		virtual void MakeToCurrent()=0;																///<切换到当前
		virtual void SwapBuffer()=0;																///<交换缓冲区
	};//class Window
}//namespace hgl
#endif//HGL_WINDOW_INCLUDE
