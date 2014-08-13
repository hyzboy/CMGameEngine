#include<hgl/platform/SystemInfo.h>
#include<hgl/platform/ConsoleSystemInitInfo.h>
#include<hgl/PlugIn.h>
#include<hgl/Info.h>
#include<hgl/Other.h>
#include<apr_general.h>

#if HGL_OS == HGL_OS_Windows
#include<wchar.h>
#include<windows.h>
#endif//HGL_OS == HGL_OS_Windows

namespace hgl
{
	namespace logger
	{
		PlugIn *InitLog();																			///<初始化日志系统
		void CloseLog();																			///<关闭日志系统

		Logger *CreateLoggerConsole	(const OSString &,LogLevel);
		Logger *CreateLoggerFile	(const OSString &,LogLevel);
//		Logger *CreateLoggerDialog	(const OSString &,LogLevel);
//		Logger *CreateLoggerNetwork	(const OSString &,LogLevel);
	}//namespace logger

	//Base/Other/MemoryPoll.CPP
	void InitMemoryPool();
	void ClearMemoryPool();

	//Base/Other/TimeCount.cpp
	void InitStartTime();

	//Base/BaseApplication/SystemInfo.CPP
	void GetCMGDKPath(CMGDKPATH &);					//取得CMGDK路径
	void SystemCheck(SystemInfo *);					//系统检测

	bool InitCore(SystemInfo &si,ConsoleSystemInitInfo *sii)
	{
		if(sii->log.disable_log==false)
		{
			AddLogger(CreateLoggerConsole	(sii->info.ProjectCode,(logger::LogLevel)sii->log.console	));
			AddLogger(CreateLoggerFile		(sii->info.ProjectCode,(logger::LogLevel)sii->log.file		));
			//AddLogger(CreateLoggerDialog	(sii->info.ProjectCode,sii->log.dialog	));
			//AddLogger(CreateLoggerNetwork	(sii->info.ProjectCode,sii->log.network	));

			InitLog();        			//初始化日志插件
		}

		const uint16 db=0xABCD;
		const uint8 *b=(const uint8 *)&db;

	#if HGL_ENDIAN == HGL_BIG_ENDIAN
		if(*b==0xCD)
	#else
		if(*b==0xAB)
	#endif//HGL_BIG_ENDIAN
			LOG_ERROR(OS_TEXT("cpu endian error,this library is bad.library macro Cpu=") HGL_CPU_NAME OS_TEXT(", Endian is ") HGL_CPU_ENDIAN);

//         #if HGL_COMPILER == HGL_COMPILER_GNU
//         if(sizeof(u16char)!=2
//          ||L'中'!=0x4E2D)
//         {
// 			LOG_ERROR(u"please use \"-fwide-exec-charset=UTF-16LE -fshort-wchar\"");
//             return(false);
//         }
//         #endif//HGL_COMPILER == HGL_COMPILER_GNU

		if(sii->info.ProjectCode.Length()<=0)
		{
			LOG_PROBLEM(OS_TEXT("please set ProjectCode,use in Registry,Filename,Network Name,Code,....."));
			return(false);
		}

		apr_initialize();			//初始化apr

#if HGL_OS == HGL_OS_Windows
		srand(GetMicroTime());
#else
		srand48(GetMicroTime());
		srand(lrand48());
#endif//

		InitStartTime();			//初始化启动时间

		InitMemoryPool();			//初始化内存池
		InitPlugIn();				//初始化插件
		InitString();

		LOG_INFO(OS_TEXT("Powered by ")+GetString(hfsName));
		LOG_INFO(OS_TEXT("Version: ")+GetString(hfsVersion));
		LOG_INFO("SDK Build time: " __DATE__ " " __TIME__);

		GetCMGDKPath(si.path);		//取得CMGDK路径

		InitPointer();

		if(sii->CheckSystem)
			SystemCheck(&si);

        return(true);
	}

	void CloseCore()
	{
		ClearAllPlugIn();

		CloseLog();
		ClearMemoryPool();

		apr_terminate();		//终止apr
	}
}//namespace hgl
