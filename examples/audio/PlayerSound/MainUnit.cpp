#include<hgl/Console.h>
#include<hgl/audio/OpenAL.h>
#include<hgl/audio/AudioBuffer.h>
#include<hgl/audio/AudioSource.h>
#include<hgl/Other.h>

using namespace hgl;
using namespace openal;

class TestObject:public ConsoleFlowObject
{
public:

    void Out(const UTF8String &name,const OpenALDevice &dev)
    {
        LOG_INFO(name+u8": \""+UTF8String(dev.name)+u8"\" Specifier: \""+UTF8String(dev.specifier)+u8"\",支持OpenAL特性版本: "+UTF8String(dev.major)+"."+UTF8String(dev.minor));
    }

    TestObject()
    {
        if(!InitOpenALDriver())
        {
            LOG_ERROR("InitOpenALDriver() return error");
            fos=hgl::fosExitGame;
            return;
        }

        LOG_INFO("InitOpenALDriver() return OK!");

        OpenALDevice default_device;

        alcGetDefaultDevice(default_device);                    //取得缺省OpenAL设备

        Out(u8"缺省OpenAL设备",default_device);

        {
            List<OpenALDevice> device_list;

            alcGetDeviceList(device_list);                      //取得所有OpenAL设备列表

            for(int i=0;i<device_list.GetCount();i++)
            {
                OpenALDevice dev;

                device_list.Get(i,dev);

                Out(u8"OpenAL设备",dev);
            }
        }

        if(!InitOpenALDevice(&default_device))                  //初始化缺省设备
            return;
    }

    ~TestObject()
    {
        CloseOpenAL();
    }

	void Update()
	{
		AudioBuffer buf;
		AudioSource source;

		buf.Load("cicada3.ogg");

        source.Link(&buf);

        source.Play();

        WaitTime(buf.Time);

		fos=hgl::fosExitGame;
	}
};//class TestObject

HGL_CONSOLE_APPLICATION("音效播放","PlayerSound",new TestObject)
