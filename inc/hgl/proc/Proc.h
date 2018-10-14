#ifndef HGL_PROCESS_INCLUDE
#define HGL_PROCESS_INCLUDE

#include<hgl/type/StringList.h>
namespace hgl
{
	/**
	* 进程管理类
	*/
	class Process																					///进程管理类
	{
		OSString work_path;
		OSString filename;
		StringList<OSString> args;

		int pid;

	public:

		Process()
		{
			pid=-1;
		}

		virtual ~Process()
		{
		}

		bool SetWorkPath(const OSString &wp);														///<设置工作目录
		bool SetExecFile(const OSString &ef);														///<设置执行文件
		void AddArgv(const OSString &argv){if(!argv.IsEmpty())args.Add(argv);}						///<增加一个参数
		void ClearArgs(){args.Clear();}																///<清除所有参数

		bool Execute();																				///<执行程序

		bool Wait();																				///<等待子进程暂停或是终止
		bool Kill();																				///<杀掉子进程
		bool RequestTerminate();																	///<请求子进程终止
	};//class Process
}//namespace hgl
#endif//HGL_PROCESS_INCLUDE
