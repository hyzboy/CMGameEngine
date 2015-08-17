#ifndef HGL_EXIT_INCLUDE
#define HGL_EXIT_INCLUDE

namespace hgl
{
	typedef int (*SignalAppExitFunc)();			//程序退出处理事件函数

	void SetSignalAppExit(SignalAppExitFunc);	//设置程序退出处理事件函数
}//namespace hgl
#endif//HGL_EXIT_INCLUDE
