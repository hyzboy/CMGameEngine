#include<hgl/platform/SystemInfo.h>
#include<hgl/audio/OpenAL.h>

namespace hgl
{
//    void SystemCheck(SystemInfo *,bool);

    class ConsoleSystemInitInfo;

    bool InitCore(SystemInfo &si, ConsoleSystemInitInfo *sii);
    void CloseCore();
}

using namespace hgl;

HGL_PLUGIN_FUNC bool InitOpenALEE(const os_char *driver_name,const char *device_name)
{
    SystemInfo si;

    InitCore(si,nullptr);                //初始化内核

    if(!openal::InitOpenAL(driver_name,device_name))
        return(false);

    return(true);
}

HGL_PLUGIN_FUNC void CloseOpenALEE()
{
    openal::CloseOpenAL();

    CloseCore();
}
