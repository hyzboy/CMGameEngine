#include<hgl/platform/SystemInfo.h>
#include<hgl/platform/ConsoleSystemInitInfo.h>
#include<hgl/PlugIn.h>
#include<hgl/Info.h>
#include<hgl/Time.h>

#if HGL_OS == HGL_OS_Windows
#include<wchar.h>
#include<windows.h>
#endif//HGL_OS == HGL_OS_Windows

namespace hgl
{
    namespace logger
    {
        PlugIn *InitLog();                                                                            ///<初始化日志系统
        void CloseLog();                                                                            ///<关闭日志系统

        Logger *CreateLoggerConsole    (const OSString &,LogLevel);
        Logger *CreateLoggerFile    (const OSString &,LogLevel);
        Logger *CreateLoggerDialog    (const OSString &,LogLevel);
//        Logger *CreateLoggerNetwork    (const OSString &,LogLevel);
    }//namespace logger
    
    //Base/DataType/Endian.cpp
    bool CheckSystemEndian();

     //Base/Other/MemoryPoll.CPP
    void InitMemoryPool();
    void ClearMemoryPool();

    //Base/BaseApplication/SystemInfo.CPP
    void GetCMGDKPath(CMGDKPATH &);                    //取得CMGDK路径
    void SystemCheck(SystemInfo *);                    //系统检测

    bool InitCore(SystemInfo &si,ConsoleSystemInitInfo *sii)
    {
        if(sii
		 &&sii->log.disable_log==false)
        {
            AddLogger(CreateLoggerConsole   (sii->info.ProjectCode,(logger::LogLevel)sii->log.console	));
            AddLogger(CreateLoggerFile		(sii->info.ProjectCode,(logger::LogLevel)sii->log.file		));

#if HGL_OS==HGL_OS_Windows
            AddLogger(CreateLoggerDialog	(sii->info.ProjectCode,(logger::LogLevel)sii->log.dialog	));
#endif//HGL_OS==HGL_OS_Windows
            //AddLogger(CreateLoggerNetwork    (sii->info.ProjectCode,sii->log.network    ));

            InitLog();                    //初始化日志插件
        }

        if(!CheckSystemEndian())
        {
            LOG_ERROR(OS_TEXT("[[[FATAL ERROR]]] CPU Endian config error,this library is bad, CPU Endian is ") HGL_CPU_ENDIAN);
            return(false);
        }

        if(sii
		 &&sii->info.ProjectCode.Length()<=0)
        {
            LOG_PROBLEM(OS_TEXT("please set ProjectCode,use in Registry,Filename,Network Name,Code,....."));
            return(false);
        }

#if HGL_OS == HGL_OS_Windows
        srand(GetMicroTime());
#else
        srand48(GetMicroTime());
        srand(lrand48());
#endif//

        InitPlugIn();                //初始化插件
        InitString();

        LOG_INFO(OS_TEXT("Powered by ")+GetString(hfsName)+OS_TEXT(" ")+GetString(hfsVersion));
        LOG_INFO("Build Time: " __DATE__ "," __TIME__);
        LOG_INFO(OS_TEXT("Build Platform: ") HGL_COMPILE_PLATFORM);
		LOG_INFO(OS_TEXT("Build Toolsets: ") HGL_COMPILE_TOOLSET);

        GetCMGDKPath(si.path);        //取得CMGDK路径

        if(sii
		 &&sii->CheckSystem)
            SystemCheck(&si);

        return(true);
    }

    void CloseCore()
    {
        ClearAllPlugIn();

        CloseLog();
    }
}//namespace hgl
