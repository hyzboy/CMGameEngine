#ifndef HGL_WinOpenGL_INCLUDE
#define HGL_WinOpenGL_INCLUDE

#include<hgl/platform/WinWindow.h>
namespace hgl
{
    /**
    * 标准Window OpenGL窗口实现
    */
    class WinGLWindow:public WinWindow
    {
        void *gl_dll;

        HGLRC glrc;

        int color_bit;
        int alpha_bit;
        int depth_bit;
        int stencil_bit;
        int accum_bit;
        int multisample_level;

        int acquire_version[2];

        bool InitOpenGL(int format=-1);

    public:

        WinGLWindow();
        virtual ~WinGLWindow();

        bool InitOpenGL(int,GraphicsSystemInitInfo::Graphics::OpenGLSetup &);
        void CloseOpenGL();

//      bool ToFullScreen(int,int,int,int,bool=false);
//      bool ToWindow(int,int);

        void SetViewport(int,int,int,int);

    public: //Windows操作系统独有部分

        HGLRC GetRC(){return glrc;}                                             ///<取得HGLRC

    public: //被实际操作系统接口层所调用的函数，请不要使用

        void SetSize(int,int);
        void MakeToCurrent();
        void SwapBuffer();
    };//class WinGLWindow
}//namespace hgl
#endif//HGL_WinOpenGL_INCLUDE
