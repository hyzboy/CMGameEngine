#ifndef HGL_X11_WINDOW_INCLUDE
#define HGL_X11_WINDOW_INCLUDE
#include<hgl/platform/Window.h>
#include<X11/X.h>
#include<X11/Xutil.h>
#include<GL/glxew.h>
namespace hgl
{
	class X11Window:public hgl::Window
	{
	protected:

		::Display *	disp;
		int			default_screen;
		::Window *	win;
		GLXContext	ctx;

		Atom		atomFullscreen;
		Atom		atomState;
		Atom		atomDeleteWindow;

	protected:

		XEvent		event,next_event;

		XWindowAttributes win_attr;

		KeySym		sym;

	protected:

		bool CreateWindow(const char *,int,int);

	public:

		X11Window();
		virtual ~X11Window();

		virtual bool InitToFullScreen(int,int,int,bool);
		virtual bool InitToWindow(int,int,GraphicsSystemInitInfo::WindowSetup &);

		virtual void Close();

		virtual bool ToMinWindow();
		virtual bool ToMaxWindow();

		virtual void Show();
		virtual void Hide();

		virtual void SetCaption(const UTF16String &);

		virtual void SetViewport(int,int,int,int);

	public:

		virtual void MakeToCurrent();
		virtual void SwapBuffer();

	public:	// X11独有部分

		virtual bool UpdateXEvent();
	};//class X11Window
}//namespace hgl
#endif//HGL_X11_WINDOW_INCLUDE
