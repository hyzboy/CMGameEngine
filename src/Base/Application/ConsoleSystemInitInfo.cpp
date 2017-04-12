#include<hgl/platform/ConsoleSystemInitInfo.h>
#include<hgl/proc/ProcMutex.h>
#include<hgl/FileSystem.h>

namespace hgl
{
    void ConsoleSystemInitInfo::InitPrivate()
    {
        mutex=new ProcMutex;

        hgl::filesystem::GetCurrentPath(ProjectPath);

        RunMode=armSync;

        log.disable_log=false;

        log.console    =llLog;
        log.file    =llLog;
        log.network    =llLog;

#ifdef _DEBUG
        log.dialog    =llHint;
#else
        log.dialog    =llProblem;
#endif//_DEBUG

        sig.pipe    =false;

        CheckDebugger=false;
        CheckSystem=false;
        MultiStartup=true;

        max_open_files=1024;
    }

    /**
    * 传入命令行参数
    * @param args 命令行参数列表
    */
    void ConsoleSystemInitInfo::Init(const OSStringList &args)
    {
        if(args.GetCount()<2)return;

        if(args.CaseFind(OS_TEXT("-no_check"))!=-1)
            CheckSystem=false;
    }

    /**
    * 系统初始化信息类构造函数
    */
    ConsoleSystemInitInfo::ConsoleSystemInitInfo()
    {
        InitPrivate();
    }

    ConsoleSystemInitInfo::~ConsoleSystemInitInfo()
    {
        delete mutex;
    }

    bool ConsoleSystemInitInfo::CheckMultiStartup()
    {
        if(MultiStartup)
            return(true);    //允许，不做处理

        const os_char *mutex_name=(os_char *)(info.ProjectCode.c_str());

        if(mutex->Create(mutex_name))
        {
            LOG_ERROR(OS_TEXT("Please don't repeat start of the current program."));
            return(false);
        }

        return(true);
    }

    /**
    * 处理未填写的值
    */
    void ConsoleSystemInitInfo::ProcBlankValue()
    {
    }
}//namespace hgl
