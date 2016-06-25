#ifndef HGL_CONSOLE_APPLICATION_INCLUDE
#define HGL_CONSOLE_APPLICATION_INCLUDE

#include<hgl/platform/BaseApplication.h>
#include<hgl/object/ConsoleFlowControl.h>
#include<hgl/type/StringList.h>
namespace hgl
{
	/**
	* 控制台应用程序基类<br>
	* 控制台应用程序基类被设计为必须输出日志
	*/
	class ConsoleApplication:public BaseApplication
	{
	protected:

		ConsoleFlowControl *flow;                                                       			///<流程控制器

	protected:

		virtual void ProcActiveObject(ConsoleFlowObject *){}

	public:

		ConsoleApplication(ConsoleFlowControl *cus_control=nullptr);
		virtual ~ConsoleApplication();

		virtual bool Init(ConsoleSystemInitInfo *);													///<初始化当前应用程序

		virtual void SetStart(ConsoleFlowObject *);													///<设定起始流程对象

        virtual void Frame()                                                                        ///<更新一帧
        {
            UpdateTime();				//更新时间

            flow->Update();             //流程刷新
            flow->ProcCurState();       //处理流程状态
        }

		virtual int Run()																		    ///<运行当前应用程序
        {
			if(!flow)return(-1);

            do
            {
                Frame();                                                                            ///<调用一帧刷新
            }
		    while(flow->ObjectState!=fosExitApp);

			return(0);
        }

		virtual void ExitApp(){if(flow)flow->ExitApp();}											///<退出游戏
	};//class ConsoleApplication
}//namespace hgl

/**
* 控制台程序总入口函数
* @param sii 系统初始化信息类，需由开发者补充填写一定的信息
* @param app 应用程序整体控制管理类
* @param args 由命令行或其它程序传来的参数列表
* @return 返回值，将会回传给操作系统
*/
extern "C" int ConsoleAppMain(hgl::ConsoleSystemInitInfo &sii,hgl::ConsoleApplication &app,const hgl::StringList<hgl::OSString> &args);
#endif//HGL_CONSOLE_APPLICATION_INCLUDE
