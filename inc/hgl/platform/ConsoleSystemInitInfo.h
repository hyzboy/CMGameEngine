#ifndef HGL_CONSOLE_SYSTEM_INIT_INFO_INCLUDE
#define HGL_CONSOLE_SYSTEM_INIT_INFO_INCLUDE

#include<hgl/type/StringList.h>
namespace hgl
{
	class ProcMutex;

	/**
	* 应用程序运行模式枚举
	*/
	enum AppRunMode     ///应用程序运行模式枚举
	{
		armNone=0,      ///<起始定义,无意义

		armASync,       ///<异步模式,Draw按FPS数量执行,Update只要有空闲就运行
		armSync,        ///<同步模式,Draw/Update均按FPS数量执行

		armFast,        ///<高速模式,Draw/Update都运行相同数量,但不做延时,一般用于测试最大fps

		armExternal,	///<外部Hinstance/hwnd模式,自身不控制主循环

		armServer,      ///<服务器程序专用运行模式

		armEnd          ///<结束定义,无意义
	};

	/**
	* 系统初始化信息类，由开发者在程序初始化时填充<br>
	*
	* ProjectPath会在这个类构造时自动写入当前路径，如果当前路径不等于游戏路径，才需要重新写入它(比如从光盘启动游戏，需要从注册表中取得游戏路径)。
	*/
	class ConsoleSystemInitInfo																		///<控制台应用系统初始化信息类
	{
	protected:

		ProcMutex *mutex;

		virtual void InitPrivate();

	public:

		OSString ProjectPath;                                                                     	///<工程路径

		struct Info
		{
			UTF8String  ProjectName;																///<工程名称,必须填写
			OSString    ProjectCode;																///<工程代码名称,必须填写
			UTF8String  ProjectVersion;                                                             ///<工程版本
			UTF8String  ProjectDeveloper;                                                           ///<工程开发者
		}info;

		AppRunMode RunMode;                                                                         ///<运行模式,默认armASync

		struct LogConfig
		{
			bool disable_log;																		///<关闭日志

			int console;																			///<日志输出到控制台
			int file;																				///<日志输出到文件
			int dialog;																				///<日志输出到对话框
			int network;																			///<日志输出到网络
		}log;

		struct SIG
		{
			bool pipe;																				///<管道信号,默认disable
		}sig;

		bool CheckDebugger;																			///<是否检测调试器,不对所有系统有效

		bool CheckSystem;																			///<是否检测系统

		bool MultiStartup;                                                                          ///<是否允许多重启动

		int max_open_files;																			///<最大可打开文件数(<=1024表示不修改)

	public:

		ConsoleSystemInitInfo();                                                                    ///<本类构造函数
		virtual ~ConsoleSystemInitInfo();                                                           ///<本类析构函数

		virtual void Init(const OSStringList &args);												///<使用外部命令行参数

		virtual void ProcBlankValue();                                                             	///<处理未填写的值

				bool CheckMultiStartup();															///<检查多重启动
	};//class ConsoleSystemInitInfo
}//namespace hgl
#endif//HGL_CONSOLE_SYSTEM_INIT_INFO_INCLUDE
