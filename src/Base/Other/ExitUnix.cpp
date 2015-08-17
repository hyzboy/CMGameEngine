#include<hgl/Exit.h>
#include<signal.h>

namespace hgl
{
	static SignalAppExitFunc app_exit_func=nullptr;

	void exit_signal_proc(int n,siginfo_t *si,void *)
	{
	}

	void SetSignalAppExit(SignalAppExitFunc func)
	{
		if(!func)return;

		app_exit_func=func;

		struct sigaction act;
        sigemptyset(&act.sa_mask);   /** 清空阻塞信号 **/

        act.sa_flags=SA_SIGINFO;     /** 设置SA_SIGINFO 表示传递附加信息到触发函数 **/
        act.sa_sigaction=exit_signal_proc;
	}
}//namespace hgl
