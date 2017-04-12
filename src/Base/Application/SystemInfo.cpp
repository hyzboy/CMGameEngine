#include<hgl/platform/SystemInfo.h>
#include<hgl/LogInfo.h>
#include<hgl/PlugIn.h>
#include<hgl/FileSystem.h>
#include<hgl/Info.h>

namespace hgl
{
    using namespace filesystem;

    CpuInfo::CpuInfo()
    {
        memset(this,0,sizeof(CpuInfo));
    }

    MemInfo::MemInfo()
    {
        memset(this,0,sizeof(MemInfo));
    }

    CMGDKPATH::CMGDKPATH()
    {
    }

    SystemInfo::SystemInfo()
    {
        memset(os_name,0,sizeof(os_name));
    }

    bool GetCMGDKPath(OSString &);
    void GetOSPath(CMGDKPATH &cp);

    bool CheckCMGDKPath(OSString &cur_path,
                        const OSString &work,
                        const OSString &start,
                        const OSString &cm,
                        const OSString &sub)
    {
        MergeFilename(cur_path,work,sub);

        if(IsDirectory(cur_path))
            return(true);

        if(work!=start)
        {
            MergeFilename(cur_path,start,sub);

            if(IsDirectory(cur_path))
                return(true);
        }

        if(work!=cm)
        {
            MergeFilename(cur_path,cm,sub);

            if(IsDirectory(cur_path))
                return(true);
        }

        OSString out_str=OS_TEXT("Don't find <")+sub+OS_TEXT("> folder from : \"")+work+OS_TEXT("\"");

        if(work!=start)    out_str+=(OS_TEXT(",\"")+start    +OS_TEXT("\""));
        if(work!=cm)    out_str+=(OS_TEXT(",\"")+cm        +OS_TEXT("\""));

        LOG_INFO(out_str);

        return(false);
    }

    void GetCMGDKPath(CMGDKPATH &cp)
    {
        OSString cur_prog;
        OSString cur_path;

        hgl::GetCurrentPath(cp.start);

        if(!GetCMGDKPath(cp.cm))
            cp.cm=cp.start;                                               //如果没有找到CMGDK系统变量

        hgl::GetCurrentProgram(cur_prog);

        cur_path=cur_prog;

        const int end_index=cur_path.FindRightChar(HGL_DIRECTORY_SEPARATOR);

        cur_path.ClipLeft(end_index);

        CheckCMGDKPath(cp.gui,      cp.start,cur_path,cp.cm,OS_TEXT("gui"));
        CheckCMGDKPath(cp.plug_ins, cp.start,cur_path,cp.cm,OS_TEXT("plug-in") + OSString(HGL_DIRECTORY_SEPARATOR) + HGL_PLATFORM_STRING);        //HGL_PLATFORM_STRING在CMAKE中定义

        GetOSPath(cp);

        hgl::info::SetString(hfsCMGDKPath,      cp.cm);
        hgl::info::SetString(hfsPlugInPath,     cp.plug_ins);
        hgl::info::SetString(hfsGUIPath,        cp.gui);
        hgl::info::SetString(hfsStartPath,      cp.start);
        hgl::info::SetString(hfsOSPath,         cp.os);
        hgl::info::SetString(hfsOSLibraryPath,  cp.library);
        hgl::info::SetString(hfsDesktopPath,    cp.mydesktop);
        hgl::info::SetString(hfsTempPath,       cp.temp);
        hgl::info::SetString(hfsUserDataPath,   cp.mydata);
        hgl::info::SetString(hfsUserProgramPath,cp.myprogram);

        hgl::info::SetString(hfsAppFileName,    cur_prog);

        LOG_INFO(   OS_TEXT("       current program: ")+cur_prog        +OS_TEXT("\n\n")
                    OS_TEXT("  program start folder: ")+cp.start        +OS_TEXT("\n\n")

                    OS_TEXT("operator system folder: ")+cp.os           +OS_TEXT("\n")
                    OS_TEXT("       os fonts folder: ")+cp.osfont       +OS_TEXT("\n")
                    OS_TEXT(" common library folder: ")+cp.library      +OS_TEXT("\n\n")

                    OS_TEXT("    common data folder: ")+cp.common_data  +OS_TEXT("\n")
                    OS_TEXT("     local data folder: ")+cp.local_data   +OS_TEXT("\n")
                    OS_TEXT("      user data folder: ")+cp.mydata       +OS_TEXT("\n\n")

                    OS_TEXT("      temp file folder: ")+cp.temp         +OS_TEXT("\n")
                    OS_TEXT("      user menu folder: ")+cp.myprogram    +OS_TEXT("\n")
                    OS_TEXT("   user desktop folder: ")+cp.mydesktop    +OS_TEXT("\n\n")

                    OS_TEXT("          CMGDK folder: ")+cp.cm           +OS_TEXT("\n")
                    OS_TEXT("      CMGDK GUI folder: ")+cp.gui          +OS_TEXT("\n")
                    OS_TEXT("  CMGDK Plug-in folder: ")+cp.plug_ins     +OS_TEXT("\n"));
    }

    struct CheckInterface
    {
        void (*Check)(SystemInfo *);
    };

    void SystemCheck(SystemInfo *si)
    {
        PlugIn *pi=LoadPlugIn(OS_TEXT("SystemCheck"));

        if(!pi)return;

        CheckInterface ci;

        pi->GetInterface(2,&ci);

        ci.Check(si);

        UnloadPlugIn(pi);
    }
}//namespace hgl
