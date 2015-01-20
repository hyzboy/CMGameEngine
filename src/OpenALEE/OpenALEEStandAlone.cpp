#include<hgl/platform/SystemInfo.h>
#include<hgl/audio/OpenAL.h>

namespace hgl
{
	void SystemCheck(SystemInfo *,bool);

	void InitCore(SystemInfo &si,bool create_log);
	void CloseCore();
}

using namespace hgl;

namespace openal
{
	bool InitOpenALEE(const os_char *driver_name,const char *device_name,bool create_log)
	{
    	SystemInfo si;

    	InitCore(si,create_log);				//初始化内核
		SystemCheck(&si,create_log);			//系统检测

		if(!InitOpenALDriver(driver_name))
			return(false);

		if(!InitOpenALDevice(device_name))
			return(false);

		return(true);
	}

	void CloseOpenALEE()
	{
    	CloseOpenAL();

		CloseCore();
	}
}
