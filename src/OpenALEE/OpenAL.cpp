#include <hgl/LogInfo.h>
#include <hgl/Info.h>
#include <hgl/FileSystem.h>
#include <hgl/audio/OpenAL.h>
#include <hgl/type/Pair.h>
#include <hgl/type/Stack.h>
#include <hgl/type/BaseString.h>
#include <hgl/type/StringList.h>
#include <hgl/ExternalModule.h>

using namespace hgl;

namespace openal
{
    static ALCchar AudioDeviceName[AL_DEVICE_NAME_MAX_LEN]={0};

    static ExternalModule *AudioEM      =nullptr;        //OpenAL动态链接库指针
    static ALCdevice *AudioDevice       =nullptr;        //OpenAL设备
    static ALCcontext *AudioContext     =nullptr;        //OpenAL上下文

    static UTF8StringList OpenALExt_List;
    static UTF8StringList OpenALContextExt_List;

    static bool AudioFloat32            =false;         //是否支持float 32数据
    static bool AudioEFX                =false;         //EFX是否可用
    static bool AudioXRAM               =false;         //X-RAM是否可用

    bool LoadALCFunc(ExternalModule *);
    bool LoadALFunc(ExternalModule *);

    bool CheckXRAM(ALCdevice_struct *);
    bool CheckEFX(ALCdevice_struct *);

    void ClearAL();
    void ClearALC();
    void ClearXRAM();
    void ClearEFX();

    void PutOpenALInfo();
    //--------------------------------------------------------------------------------------------------
    /**
    * 加载OpenAL
    * @return 加载OpenAL是否成功
    */
    bool LoadOpenAL(const os_char *filename)
    {
        const os_char oalfn[][HGL_MAX_PATH]=
        {
#if HGL_OS == HGL_OS_Windows
            OS_TEXT("\\OpenAL32.dll"),
            OS_TEXT("\\ct_oal.dll"),
            OS_TEXT("\\nvOpenAL.dll"),
            OS_TEXT("\\soft_oal.dll"),
            OS_TEXT("\\wrap_oal.dll"),
#elif (HGL_OS == HGL_OS_Linux)||(HGL_OS == HGL_OS_FreeBSD)||(HGL_OS == HGL_OS_NetBSD)||(HGL_OS == HGL_OS_OpenBSD)
            OS_TEXT("/libopenal.so"),
            OS_TEXT("/libopenal.so.1"),
#elif HGL_OS == HGL_OS_MacOS
            OS_TEXT("/libopenal.dylib"),
#endif//HGL_OS == HGL_OS_Windows
            OS_TEXT("\x0")
        };

        int count=0;
        const os_char *pi_path=GetString(hfsPlugInPath);
        os_char pifn[HGL_MAX_PATH];
        os_char dllfn[HGL_MAX_PATH];
        os_char *final_filename=nullptr;

        CloseOpenAL();

        if(filename)
        {
            hgl::strcpy(dllfn,HGL_MAX_PATH,hgl::info::GetString(hgl::info::hfsOSLibraryPath).c_str());
            hgl::strcat(dllfn,HGL_MAX_PATH,HGL_DIRECTORY_SEPARATOR);
            hgl::strcat(dllfn,HGL_MAX_PATH,filename,HGL_MAX_PATH);

            hgl::strcpy(pifn,HGL_MAX_PATH,pi_path);
            hgl::strcat(pifn,HGL_MAX_PATH,HGL_DIRECTORY_SEPARATOR);
            hgl::strcat(pifn,HGL_MAX_PATH,filename,HGL_MAX_PATH);

            if(filesystem::FileConfirm(filename ))final_filename=(os_char *)filename;else
            if(filesystem::FileConfirm(dllfn    ))final_filename=dllfn;else
            if(filesystem::FileConfirm(pifn        ))final_filename=pifn;

            AudioEM=LoadExternalModule(final_filename);
        }
        else
        {
            do
            {
                hgl::strcpy(pifn,HGL_MAX_PATH,pi_path);
                hgl::strcat(pifn,HGL_MAX_PATH,oalfn[count],HGL_MAX_PATH);

                hgl::strcpy(dllfn,HGL_MAX_PATH,hgl::info::GetString(hgl::info::hfsOSLibraryPath).c_str());
                hgl::strcat(dllfn,HGL_MAX_PATH,oalfn[count],HGL_MAX_PATH);

                if(filesystem::FileConfirm(dllfn))final_filename=dllfn;else
                if(filesystem::FileConfirm(pifn ))final_filename=pifn;else
                    continue;

                AudioEM=LoadExternalModule(final_filename);

                if(AudioEM)break;      //如果加载成功

            }while(oalfn[++count][0]);
        }

        if(AudioEM)
        {
            LOG_INFO(OS_TEXT("加载OpenAL成功！使用动态链接库: ")+OSString(final_filename));

            return (AL_TRUE);
        }
        else
        {
        #if HGL_OS == HGL_OS_Windows
            LOG_ERROR(  OS_TEXT("加载OpenAL动态链接库失败！OpenAL动态链接库可能是: OpenAL32.dll、soft_oal.dll、wrap_oal.dll、ct_oal.dll、nvOpenAL.dll\n")
                        OS_TEXT("软件实现的OpenAL32.DLL、wrap_oal可在http://www.openal.org上下载、soft_oal.dll可在http://kcat.strangesoft.net/openal.html下载!\n")
                        OS_TEXT("硬件实现的OpenAL32.DLL请到对应您声卡的厂商网站下载对应的驱动程序! 下载完成后可其放入Windows\\System32目录下或应用程序Plug-Ins目录下即可!"));
        #else
            LOG_ERROR(  OS_TEXT("加载OpenAL动态链接库失败！"));
        #endif//#if HGL_OS == HGL_OS_Windows
            return (AL_FALSE);
        }
    }

    const char *alcGetDeviceNameList()
    {
        if(!alcIsExtensionPresent)return(nullptr);

        if(alcIsExtensionPresent(nullptr,"ALC_ENUMERATE_ALL_EXT"))
            return (char *)alcGetString(nullptr,ALC_ALL_DEVICES_SPECIFIER);
        else
        if(alcIsExtensionPresent(nullptr,"ALC_ENUMERATION_EXT"))
            return (char *)alcGetString(nullptr,ALC_DEVICE_SPECIFIER);
        else
            return(nullptr);
    }

    bool alcGetDefaultDeviceName(char *name)
    {
        if(alcIsExtensionPresent)
        {
            const char *result=nullptr;

            if(alcIsExtensionPresent(nullptr,"ALC_ENUMERATE_ALL_EXT"))
                result=(char *)alcGetString(nullptr,ALC_DEFAULT_ALL_DEVICES_SPECIFIER);
            else
            if(alcIsExtensionPresent(nullptr,"ALC_ENUMERATION_EXT"))
                result=(char *)alcGetString(nullptr,ALC_DEFAULT_DEVICE_SPECIFIER);

            if(result&&(*result))
            {
                hgl::strcpy(name,AL_DEVICE_NAME_MAX_LEN,result);
                return(true);
            }
        }

        *name=0;
        return(false);
    }

    bool alcGetDeviceInfo(OpenALDevice &dev,const char *device_name)
    {
        ALCdevice *device=alcOpenDevice(device_name);

        if(!device)
            return(false);

        hgl::strcpy(dev.name,       AL_DEVICE_NAME_MAX_LEN,device_name);
        hgl::strcpy(dev.specifier,  AL_DEVICE_NAME_MAX_LEN,alcGetString(device,ALC_DEVICE_SPECIFIER));

        alcGetIntegerv(device,ALC_MAJOR_VERSION,sizeof(int),&dev.major);

        if(dev.major)
        {
            alcGetIntegerv(device,ALC_MINOR_VERSION,sizeof(int),&dev.minor);
        }
        else
        {
            dev.major=1;
            dev.minor=0;
        }

        alcCloseDevice(device);
        return(true);
    }

    bool alcGetDefaultDevice(OpenALDevice &dev)
    {
        if(!alcGetDefaultDeviceName(dev.name))
            return(false);

        return alcGetDeviceInfo(dev,dev.name);
    }

    int alcGetDeviceList(List<OpenALDevice> &device_list)
    {
        const char *devices=alcGetDeviceNameList();

        if(!devices)
        {
            LOG_INFO(OS_TEXT("取得的OpenAL设备列表为空！"));
            return(-1);
        }

        while(*devices)
        {
            OpenALDevice dev;

            if(alcGetDeviceInfo(dev,devices))
                device_list.Add(dev);

            devices+=::strlen(devices)+1;
        }

        return device_list.GetCount();
    }

    void EnumOpenALDevice()
    {
        const char *devices=alcGetDeviceNameList();
        const char *actual_devicename;

        if(!devices)
        {
            LOG_INFO(OS_TEXT("取得的OpenAL设备列表为空！"));
            return;
        }

        while(*devices)
        {
            ALCdevice *device=alcOpenDevice(devices);

            if(device)
            {
                actual_devicename=alcGetString(device,ALC_DEVICE_SPECIFIER);

                int major=0,minor=0;

                alcGetIntegerv(device,ALC_MAJOR_VERSION,sizeof(int),&major);

                if(major)
                {
                    alcGetIntegerv(device,ALC_MINOR_VERSION,sizeof(int),&minor);

                    LOG_INFO(u8"OpenAL设备: "+UTF8String(devices)+u8" Specifier:"+UTF8String(actual_devicename)+u8",支持OpenAL特性版本: "+UTF8String(major)+"."+UTF8String(minor));
                }
                else
                {
                    LOG_INFO(u8"OpenAL设备: "+UTF8String(devices)+u8" Specifier:"+UTF8String(actual_devicename)+u8",不支持版本获取，按OpenAL 1.0标准执行");
                }

                alcCloseDevice(device);
            }

            devices+=::strlen(devices)+1;
        }
    }

    void alcSetDefaultContext()
    {
        alcMakeContextCurrent(AudioContext);
    }

    void GetOpenALExt(UTF8StringList &ext_list,const char *ext_str)
    {
        int len=hgl::strlen(ext_str);

        SplitToStringListBySpace(ext_list,ext_str,len);         //space指所有不可打印字符
    }

    void InitOpenALExt()
    {
        GetOpenALExt(OpenALExt_List,alGetString(AL_EXTENSIONS));
        GetOpenALExt(OpenALContextExt_List,alcGetString(AudioDevice,ALC_EXTENSIONS));

        if(OpenALExt_List.Find("AL_EXT_FLOAT32")!=-1)
            AudioFloat32=true;
    }

    /**
    * 初始化OpenAL驱动
    * @param driver_name 驱动名称,如果不写则自动查找
    * @return 是否初始化成功
    */
    bool InitOpenALDriver(const os_char *driver_name)
    {
        if (!AudioEM)
            if (!LoadOpenAL(driver_name))RETURN_FALSE;

        RETURN_BOOL(LoadALCFunc(AudioEM));
    }

    bool InitOpenALDeviceByName(const char *device_name)
    {
        bool default_device=false;

        if(device_name)
            hgl::strcpy(AudioDeviceName,AL_DEVICE_NAME_MAX_LEN,device_name);
        else
        {
            alcGetDefaultDeviceName(AudioDeviceName);

            if(*AudioDeviceName)
            {
                LOG_INFO(u8"没有指定音频设备，按缺省设备初始化，可能不是最佳选择："+UTF8String(AudioDeviceName));
            }
            else
            {
                LOG_INFO(OS_TEXT("没有指定音频设备，按缺省设备初始化，可能不是最佳选择！也有可能初始化失败！"));
            }

            default_device=true;
        }

        AudioDevice=alcOpenDevice(AudioDeviceName);

        if(AudioDevice==nullptr)
        {
            LOG_ERROR(u8"打开音频设备失败: "+UTF8String(AudioDeviceName));

            if(default_device)
            {
                CloseOpenAL();
                return(AL_FALSE);
            }

            //使用缺省设备
            {
                alcGetDefaultDeviceName(AudioDeviceName);
                AudioDevice=alcOpenDevice(AudioDeviceName);

                if(!AudioDevice)
                {
                    LOG_ERROR(OS_TEXT("使用指定设备和缺省设备均无法正常初始化音频设备！"));
                    CloseOpenAL();
                    return(AL_FALSE);
                }

                LOG_INFO(OS_TEXT("指定音频设备初始化失败，改用缺省设备！"));
            }
        }

        LOG_INFO(U8_TEXT("打开音频设备成功: ")+UTF8String(AudioDeviceName));

        AudioContext=alcCreateContext(AudioDevice,0);
        if(AudioContext==nullptr)
        {
            LOG_ERROR(OS_TEXT("关联音频设备上下文失败！"));
            CloseOpenAL();
            return (AL_FALSE);
        }
        #ifdef _DEBUG
        else
        {
            LOG_INFO(OS_TEXT("关联音频设备成功！"));
        }
        #endif//_DEBUG

        if(!alcMakeContextCurrent(AudioContext))
        {
            LOG_ERROR(OS_TEXT("关联音频设备上下文与音频设备失败！"));
            CloseOpenAL();
            RETURN_FALSE;
        }
        #ifdef _DEBUG
        else
        {
            LOG_INFO(OS_TEXT("关联音频设备上下文与音频设备成功！"));
        }
        #endif//_DEBUG

        hgl::strcpy(AudioDeviceName,AL_DEVICE_NAME_MAX_LEN,alcGetString(AudioDevice,ALC_DEVICE_SPECIFIER));

        LOG_INFO(u8"初始化音频设备完成: "+UTF8String(AudioDeviceName));

        if (!LoadALFunc(AudioEM))
        {
            LOG_INFO(OS_TEXT("加载OpenAL函数失败！"));
            CloseOpenAL();
            RETURN_FALSE;
        }

        LOG_INFO(OS_TEXT("加载OpenAL函数完成！"));

        if(alcIsExtensionPresent)
        {
            AudioXRAM=CheckXRAM(AudioDevice);
            AudioEFX=CheckEFX(AudioDevice);
        }
        else
        {
            AudioXRAM=false;
            AudioEFX=false;
        }

        alGetError();

        InitOpenALExt();

        LOG_INFO(OS_TEXT("初始化OpenAL完成！"));
        PutOpenALInfo();

        return(AL_TRUE);
    }

    bool InitOpenALDevice(const OpenALDevice *device)
    {
        if(device)
            return InitOpenALDeviceByName(device->name);
        else
            return InitOpenALDeviceByName(nullptr);
    }

    //--------------------------------------------------------------------------------------------------
    /**
    * 关闭OpenAL
    */
    void CloseOpenAL()
    {
        if(AudioEM)
        {
            alcMakeContextCurrent(0);

            if(AudioContext) alcDestroyContext(AudioContext);
            if(AudioDevice)    alcCloseDevice(AudioDevice);
            SAFE_CLEAR(AudioEM);

            LOG_INFO(OS_TEXT("关闭OpenAL完成！"));
        }

        ClearAL();
        ClearALC();
        ClearXRAM();
        ClearEFX();
    }

    /**
    * 设置距离衰减模型(默认钳位倒数距离模型)
    */
    bool SetDistanceModel(ALenum distance_model)
    {
        if (!AudioEM)return(false);
        if (!alDistanceModel)return(false);

        if (distance_model<AL_INVERSE_DISTANCE
          ||distance_model>AL_EXPONENT_DISTANCE_CLAMPED)
            return(false);

        alDistanceModel(distance_model);
        return(true);
    }

    bool SetSpeedOfSound(const float ss)
    {
        if (!AudioEM)return(false);
        if (!alSpeedOfSound)return(false);

        alSpeedOfSound(ss);
        return(true);
    }

    double SetSpeedOfSound(const double height,const double temperature,const double humidity)
    {
        if (!AudioEM)return(0);
        if (!alSpeedOfSound)return(0);

        double v=HGL_SPEED_OF_SOUND;                        //海拔0米，0摄氏度

        v*=sqrt(1.0f+temperature/(-HGL_ABSOLUTE_ZERO));     //先计算温度

        ////11000米高度，温度-56.5，音速295m/s。需根据大气密度来计算

        //另需计算温度，影响在0.1-0.6之间。

        alSpeedOfSound(v/HGL_SPEED_OF_SOUND);
        return(v);
    }

    bool SetDopplerFactor(const float df)
    {
        if (!AudioEM)return(false);
        if (!alDopplerFactor)return(false);

        alDopplerFactor(df);
        return(true);
    }

    bool SetDopplerVelocity(const float dv)
    {
        if (!AudioEM)return(false);
        if (!alDopplerVelocity)return(false);

        alDopplerVelocity(dv);
        return(true);
    }
    //--------------------------------------------------------------------------------------------------
//    void *alcGetCurrentDevice() { return (AudioDevice); }
//    char *alcGetCurrentDeviceName() { return(AudioDeviceName); }

    const struct ALFormatBytes
    {
        ALenum format;
        uint bytes;
    }
    al_format_bytes[]=
    {
        {AL_FORMAT_MONO8,           1},
        {AL_FORMAT_MONO16,          2},
        {AL_FORMAT_STEREO8,         2},
        {AL_FORMAT_STEREO16,        4},

//         {AL_FORMAT_QUAD16,            8},
//         {AL_FORMAT_51CHN16,            12},
//         {AL_FORMAT_61CHN16,            14},
//         {AL_FORMAT_71CHN16,            16},
//
        {AL_FORMAT_MONO_FLOAT32,    4},
        {AL_FORMAT_STEREO_FLOAT32,  8},
//
//         {AL_FORMAT_QUAD8,            4},
//         {AL_FORMAT_QUAD16,            8},
//         {AL_FORMAT_QUAD32,            16},
//         {AL_FORMAT_REAR8,            4},
//         {AL_FORMAT_REAR16,            8},
//         {AL_FORMAT_REAR32,            16},
//         {AL_FORMAT_51CHN8,            6},
//         {AL_FORMAT_51CHN16,            12},
//         {AL_FORMAT_51CHN32,            24},
//         {AL_FORMAT_61CHN8,            7},
//         {AL_FORMAT_61CHN16,            14},
//         {AL_FORMAT_61CHN32,            28},
//         {AL_FORMAT_71CHN8,            8},
//         {AL_FORMAT_71CHN16,            16},
//         {AL_FORMAT_71CHN32,            32},

        {0,0}
    };

    const int al_get_format_byte(ALenum format)
    {
        const ALFormatBytes *p=al_format_bytes;

        while(p->format)
        {
            if(p->format==format)
                return p->bytes;

            ++p;
        }

        return(0);
    }

    /**
    * 计算音频数据可播放的时间
    * @param size 数据字节长度
    * @param format 数据格式
    * @param freq 数据采样率
    * @return 时间(秒)
    */
    double AudioDataTime(ALuint size,ALenum format,ALsizei freq)
    {
        if(size==0||freq==0)return(0);

        const uint byte=al_get_format_byte(format);

        if(byte==0)
            return(0);

        return (double(size)/double(freq)/double(byte));
    }

    /**
    * 计算一秒钟音频数据所需要的字节数
    * @param format 数据格式
    * @param freq 数据采样率
    * @return 时间(秒)
    */
    unsigned int AudioTime(ALenum format,ALsizei freq)
    {
        const uint byte=al_get_format_byte(format);

        if(byte==0)
            return(0);

        return byte*freq;
    }

    unsigned int GetMaxNumSources()
    {
        ALuint source;
        Stack<ALuint> source_stack;

        // Clear AL Error Code
        alGetError();

        for(;;)
        {
            alGenSources(1, &source);

            if (alGetError() != AL_NO_ERROR)
                break;

            source_stack.Push(source);
        }

        const int count=source_stack.GetCount();
        const ALuint *p=source_stack.GetData();

        alDeleteSources(count,p);

        if (alGetError() != AL_NO_ERROR)
        {
            for(int i=0;i<count;i++)
                alDeleteSources(1, p++);
        }

        return count;
    }

    /**
     * 是否支持浮点音频数据
     */
    bool IsSupportFloatAudioData()
    {
        return AudioFloat32;
    }
    //--------------------------------------------------------------------------------------------------
    const char *alGetErrorInfo(const char *filename,const int line)
    {
        if(!alGetError)return(U8_TEXT("OpenALEE/OpenAL 未初始化!"));

        const char *result=nullptr;

        const char al_error_invalid             []=U8_TEXT("invalid");
        const char al_error_invalid_name        []=U8_TEXT("invalid name");
        const char al_error_invalid_enum        []=U8_TEXT("invalid enum");
        const char al_error_invalid_value       []=U8_TEXT("invalid value");
        const char al_error_invalid_operation   []=U8_TEXT("invalid operation");
        const char al_error_out_of_memory       []=U8_TEXT("out of memory");
        const char al_error_unknown_error       []=U8_TEXT("unknown error");

        const ALenum error=alGetError();

        if(error==AL_NO_ERROR           )result=nullptr;else
        if(error==AL_INVALID            )result=al_error_invalid;else
        if(error==AL_INVALID_NAME       )result=al_error_invalid_name;else
        if(error==AL_INVALID_ENUM       )result=al_error_invalid_enum;else
        if(error==AL_INVALID_VALUE      )result=al_error_invalid_value;else
        if(error==AL_INVALID_OPERATION  )result=al_error_invalid_operation;else
        if(error==AL_OUT_OF_MEMORY      )result=al_error_out_of_memory;else
                                         result=al_error_unknown_error;

        if(result)
        {
            LOG_ERROR(U8_TEXT("OpenAL错误,source file:\"")+UTF8String(filename)+u8"\",line:"+UTF8String(line));
            LOG_ERROR(U8_TEXT("OpenAL ErrorNo:")+UTF8String(result));
        }

        return(result);
    }

    void PutOpenALInfo()
    {
        if(!alGetString)return;

        LOG_INFO(OS_TEXT("OpenAL 信息:"));

        LOG_INFO(UTF8String(u8"　　　音频芯片制造商: ")+alGetString(AL_VENDOR    ));
        LOG_INFO(UTF8String(u8"　　支持的OpenAL版本: ")+alGetString(AL_VERSION   ));
        LOG_INFO(UTF8String(u8"　　　　音频芯片名称: ")+alGetString(AL_RENDERER  ));
        LOG_INFO(OS_TEXT(               "　　　　　最大音源数: ")+OSString(GetMaxNumSources()));
        LOG_INFO(AudioFloat32?OS_TEXT(  "    浮点音频数据支持: 是")
                             :OS_TEXT(  "    浮点音频数据支持: 否"));

        LOG_INFO(AudioEFX?OS_TEXT(  "　　　　　   EFX支持: 是")
                         :OS_TEXT(  "　　　　　   EFX支持: 否"));
        LOG_INFO(AudioXRAM?OS_TEXT( "           X-RAM支持: 是")
                          :OS_TEXT( "           X-RAM支持: 否"));

        if(AudioXRAM)
        {
            int size;

            alcGetIntegerv(AudioDevice,eXRAMSize,sizeof(int),&size);

            LOG_INFO(OS_TEXT("           X-RAM容量: ")+OSString(size));
        }

        for(int i=0;i<OpenALExt_List.GetCount();i++)
            if(i==0)
            {
                LOG_INFO(UTF8String(u8"　　支持的OpenAL扩展: ")+OpenALExt_List[i]);
            }
            else
            {
                LOG_INFO(UTF8String(u8"                      ")+OpenALExt_List[i]);
            }

        for(int i=0;i<OpenALContextExt_List.GetCount();i++)
            if(i==0)
            {
                LOG_INFO(UTF8String(u8"支持的OpenAL设备扩展: ")+OpenALContextExt_List[i]);
            }
            else
            {
                LOG_INFO(UTF8String(u8"                      ")+OpenALContextExt_List[i]);
            }


        #ifdef _DEBUG
        LOG_INFO(OS_TEXT("输出OpenAL信息完成"));
        #endif//
    }

    /**
    * 初始化OpenAL
    * @param driver_name 驱动名称,如果不写则自动查找
    * @param device_name 设备名称,如果不写则自动选择缺省设备
    * @param out_info   是否输出当前OpenAL设备信息
    * @param enum_device 是否枚举所有的设备信息输出
    * @return 是否初始化成功
    */
    bool InitOpenAL(const os_char *driver_name,const char *device_name,bool out_info,bool enum_device)
    {
        if (!InitOpenALDriver(driver_name))
            return(false);

        if (enum_device)
            EnumOpenALDevice();

        if (!InitOpenALDeviceByName(device_name))
        {
            CloseOpenAL();
            return(false);
        }

        InitOpenALExt();

        if(out_info)
            PutOpenALInfo();

        return(true);
    }
}//namespace openal
