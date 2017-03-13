#ifndef HGL_X11_WINDOW_INCLUDE
#define HGL_X11_WINDOW_INCLUDE
#include<hgl/platform/Window.h>
#include<X11/X.h>
#include<X11/Xutil.h>
#include<GL/glxew.h>
namespace hgl
{
    namespace platform
    {
        class X11Window:public Window
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

            bool CreateWindow(const char *,int,int,const OpenGLSetup *gs);

        public:

            X11Window();
            ~X11Window();

            bool InitToFullScreen(int,int,int,bool,const OpenGLSetup *);
            bool InitToWindow(int,int,const char *,const OpenGLSetup *);

            void Close();

            bool ToMin()override;
            bool ToMax()override;

            void Show()override;
            void Hide()override;

            void SetCaption(const OSString &)override;
            void SetMouseCoord(const Vertex2i &coord)override;
            void SetSize(int w,int h)override;
//             void SetViewport(int,int,int,int);

        public:

            void MakeToCurrent()override;
            void SwapBuffer()override;
            void WaitEvent(const double &time_out)override;
            void PollEvent()override;
            bool IsOpen()override;
            void InitProcEvent(AppEventBase *)override;
        };//class X11Window
    }//namespace platform
}//namespace hgl
#endif//HGL_X11_WINDOW_INCLUDE
