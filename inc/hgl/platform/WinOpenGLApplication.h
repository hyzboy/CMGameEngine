#ifndef HGL_WIN_OPENGL_APPLICATION_INCLUDE
#define HGL_WIN_OPENGL_APPLICATION_INCLUDE

#include<hgl/platform/OpenGLApplication.h>
namespace hgl
{
	class WinGLWindow;
	class GraphicsSystemInitInfo;

	/**
	* 标准Windows OpenGL图形应用程序类
	*/
	class WinOpenGLApplication:public OpenGLApplication
	{
		WinGLWindow *glwin;

		double prev_time;

		bool InitVideoMode();
		bool InitOpenGLforWindows();

		void RunSync();
		void RunASync();
		void RunFast();

	public:

		AppRunMode  RunMode;                                                                        ///<运行模式

		bool        BackNotDraw;                                                                    ///<当程序位于后台不刷新画面
		bool        WaitActive;                                                                     ///<等待程序被切换至活动状态

	public:

		WinOpenGLApplication();
		~WinOpenGLApplication();

		bool 			Init(GraphicsSystemInitInfo *);												///<初始化系统
		WinGLWindow *	GetOpenGLWindow(){return glwin;}											///<取得Windows OpenGL窗口

        void			ProcMessage();                                                              ///<处理Windows消息
        void			SwapBuffer();																///<交换Windows OpenGL缓冲区

        virtual void Frame()
        {
            OpenGLApplication::Frame();                     //调用OpenGL应用一帧刷新函数

            ProcMessage();                                  //处理Windows消息
        }

		virtual int Run()
        {
			if(!flow)return(-1);

            do
            {
                UpdateTime();                               //更新时间

                ProcMessage();                              //处理Windows消息
                                                            //调用相应的运行处理函数
                if(RunMode==armASync)RunASync();else
                if(RunMode==armFast	)RunFast();else
                    RunSync();
            }
            while(flow->ObjectState!=fosExitApp);

			return(0);
        }
	};//class WinOpenGLApplication
}//namespace hgl
#endif//HGL_WIN_OPENGL_APPLICATION_INCLUDE
