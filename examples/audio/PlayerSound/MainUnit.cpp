#include<hgl/Console.h>
#include<hgl/audio/OpenAL.h>
#include<hgl/audio/AudioBuffer.h>
#include<hgl/audio/AudioSource.h>
#include<hgl/Other.h>

using namespace hgl;
using namespace openal;

class TestObject:public ConsoleFlowObject
{
	void Update()
	{
		if(!InitOpenALDriver())
		{
			std::cerr<<"InitOpenALDriver() return error"<<std::endl;
			fos=hgl::fosExitGame;
			return;
		}

		std::cout<<"InitOpenALDriver() return OK!"<<std::endl;

		char default_device[256]="";

		if(alcGetDefaultDevice(default_device))
			LOG_INFO(u8"OpenAL缺省设备: "+UTF8String(default_device));

		const char *devices=alcGetDeviceList();

		while(*devices)
		{
			ALCdevice *device=alcOpenDevice(devices);

			if(device)
			{
				const char *actual_devicename=alcGetString(device,ALC_DEVICE_SPECIFIER);

				int major=0,minor=0;
				int ver;

				alcGetIntegerv(device,ALC_MAJOR_VERSION,sizeof(int),&major);

				if(major)
				{
					alcGetIntegerv(device,ALC_MINOR_VERSION,sizeof(int),&minor);

					LOG_INFO(u8"OpenAL设备: \""+UTF8String(devices)+u8"\" Specifier: \""+UTF8String(actual_devicename)+u8"\",支持OpenAL特性版本: "+UTF8String(major)+"."+UTF8String(minor));
				}
				else
				{
					LOG_INFO(u8"OpenAL设备: \""+UTF8String(devices)+u8"\" Specifier: \""+UTF8String(actual_devicename)+u8"\",不支持版本获取，按OpenAL 1.0标准执行");
				}

				alcCloseDevice(device);
			}

			devices+=::strlen(devices)+1;
		}

		if(!InitOpenALDevice(default_device))
			return;

		AudioBuffer buf;
		AudioSource source;

		buf.Load("cicada3.ogg");

        source.Link(&buf);

        source.Play();

        WaitTime(buf.Time);

		fos=hgl::fosExitGame;
	}

    ~TestObject()
    {
        CloseOpenAL();
    }
};//class TestObject

HGL_CONSOLE_APPLICATION("音效播放","PlayerSound",new TestObject)
