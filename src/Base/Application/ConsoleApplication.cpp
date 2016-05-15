#include<hgl/platform/ConsoleApplication.h>
#include<hgl/object/ConsoleFlowControl.h>
#include<hgl/platform/ConsoleSystemInitInfo.h>
#include<hgl/Info.h>

#if HGL_OS == HGL_OS_Windows
	#include<windows.h>
#else
	#include<signal.h>
	#include<sys/resource.h>
#endif//HGL_OS == HGL_OS_Windows

namespace hgl
{
	/**
	* 控制台应用程序基类构造函数
	* @param fc 自定义流程控制器
	* @param log 是否创建日志
	*/
	ConsoleApplication::ConsoleApplication(ConsoleFlowControl *cus_control)
	{
		if(cus_control)
			flow=cus_control;
		else
			flow=new ConsoleFlowControl();

		SetEventCall(flow->OnChange,this,ConsoleApplication,ProcActiveObject);
	}

	ConsoleApplication::~ConsoleApplication()
	{
		SAFE_CLEAR(flow);
	}

	bool ConsoleApplication::Init(ConsoleSystemInitInfo *_sii)
	{
		sii=_sii;

		if(sii)
		{
			if(!BaseApplication::Init(sii))
				return(false);

#if HGL_OS == HGL_OS_Windows
			if(sii->CheckDebugger&&IsDebuggerPresent())
			{
				LOG_ERROR(OS_TEXT("本程序不能运行在调试模式下!"));
				return(false);
			}
#else
			if(sii->sig.pipe==false)
			{
				signal(SIGPIPE,SIG_IGN);			//屏蔽管道信号,一般send/recv一个断开的Socket会触发此信号，导致进程退出
			}

			{
				struct rlimit64 rl;

				getrlimit64(RLIMIT_NOFILE,&rl);

				if(sii->max_open_files>rl.rlim_cur)
				{
					if(sii->max_open_files>rl.rlim_cur)
					{
						if(sii->max_open_files>rl.rlim_max)
							rl.rlim_cur=rl.rlim_max;
						else
							rl.rlim_cur=sii->max_open_files;

						if(setrlimit64(RLIMIT_NOFILE,&rl))
						{
							LOG_ERROR(OS_TEXT("Set Max Open file maximum value to ")+OSString((uint64)(rl.rlim_cur))+OS_TEXT("error."));
							return(false);
						}
					}
				}
			}
#endif//HGL_OS == HGL_OS_Windows
		}

        if(flow)
    		return(true);
        else
            return(false);
	}

	void ConsoleApplication::SetStart(ConsoleFlowObject *fo)
	{
		flow->SetStart(fo);

		ProcActiveObject(fo);
	}
}//namespace hgl
