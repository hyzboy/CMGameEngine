#include <hgl/LogInfo.h>
#include <hgl/Info.h>
#include <hgl/File.h>
#include <hgl/Other.h>
#include <hgl/audio/OpenAL.h>
#include <hgl/type/Pair.h>
#include <hgl/type/BaseString.h>
#include <hgl/ExternalModule.h>

using namespace hgl;

namespace openal
{
	static ALCchar AudioDeviceName[256]={0};

	static ExternalModule *AudioEM	=nullptr;		//OpenAL动态链接库指针
	static ALCdevice *AudioDevice	=nullptr;		//OpenAL设备
	static ALCcontext *AudioContext	=nullptr;		//OpenAL上下文

	static bool AudioEFX			=false;			//EFX是否可用
	static bool AudioXRAM			=false;			//X-RAM是否可用

	void LoadALCFunc(ExternalModule *);
	void LoadALFunc(ExternalModule *);

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
			OS_TEXT("\\OpenAL32.DLL"),
			OS_TEXT("\\wrap_oal.DLL"),
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
			hgl::strcpy(dllfn,hgl::info::GetString(hgl::info::hfsOSLibraryPath).c_str());
			hgl::strcat(dllfn,HGL_DIRECTORY_SEPARATOR);
			hgl::strcat(dllfn,filename);

			hgl::strcpy(pifn,pi_path);
			hgl::strcat(pifn,HGL_DIRECTORY_SEPARATOR);
			hgl::strcat(pifn,filename);

			if(FileConfirm(filename ))final_filename=(os_char *)filename;else
			if(FileConfirm(dllfn    ))final_filename=dllfn;else
			if(FileConfirm(pifn	    ))final_filename=pifn;

			AudioEM=LoadExternalModule(final_filename);
		}
		else
		{
			do
			{
				hgl::strcpy(pifn,pi_path);
				hgl::strcat(pifn,oalfn[count]);

				hgl::strcpy(dllfn,hgl::info::GetString(hgl::info::hfsOSLibraryPath).c_str());
				hgl::strcat(dllfn,oalfn[count]);

				if(FileConfirm(dllfn))final_filename=dllfn;else
				if(FileConfirm(pifn ))final_filename=pifn;else
					continue;

				AudioEM=LoadExternalModule(final_filename);

				if(AudioEM)break;      //如果加载成功

			}while(oalfn[++count][0]);
		}

		if(AudioEM)
		{
			LOG_ERROR(OS_TEXT("加载OpenAL成功！使用动态链接库: ")+OSString(final_filename));

			return (AL_TRUE);
		}
		else
		{
		#if HGL_OS == HGL_OS_Windows
			LOG_ERROR(	OS_TEXT("加载OpenAL动态链接库失败！OpenAL动态链接库可能是: OpenAL32.DLL、wrap_oal.DLL、ct_oal.DLL、nvOpenAL.DLL\n")
						OS_TEXT("软件实现的OpenAL32.DLL、wrap_oal可在http://www.openal.org上下载!\n")
						OS_TEXT("硬件实现的OpenAL32.DLL请到对应您声卡的厂商网站下载对应的驱动程序!\n")
						OS_TEXT("下载完成后可其放入Windows\\System32目录下或Plug-Ins目录下即可!"));
		#else
			LOG_ERROR(	OS_TEXT("加载OpenAL动态链接库失败！"));
		#endif//#if HGL_OS == HGL_OS_Windows
			return (AL_FALSE);
		}
	}

	const char *alcGetDeviceList()
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

	bool alcGetDefaultDevice(char *name)
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
				hgl::strcpy(name,result);
				return(true);
			}
		}

		*name=0;
		return(false);
	}

	void EnumOpenALDevice()
	{
		const char *devices=alcGetDeviceList();
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

	/**
	* 初始化OpenAL驱动
	* @param driver_name 驱动名称,如果不写则自动查找
	* @return 是否初始化成功
	*/
	bool InitOpenALDriver(const os_char *driver_name)
	{
		if(!AudioEM)
			if(!LoadOpenAL(driver_name))return(AL_FALSE);

		LoadALCFunc(AudioEM);

		return(true);
	}

	/**
	* 初始化OpenAL设备
	* @param device_name 设备名称,如果不写则自动选择缺省设备
	* @return 是否初始化成功
	*/
	bool InitOpenALDevice(const char *device_name)
	{
    	bool default_device=false;

		if(device_name)
			hgl::strcpy(AudioDeviceName,device_name);
		else
		{
			alcGetDefaultDevice(AudioDeviceName);

			if(*AudioDeviceName)
			{
				LOG_INFO(UTF8String("没有指定音频设备，按缺省设备初始化，可能不是最佳选择：")+UTF8String(AudioDeviceName));
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
			LOG_ERROR(UTF8String("打开音频设备失败: ")+AudioDeviceName);

			if(default_device)
			{
				CloseOpenAL();
				return(AL_FALSE);
			}

			//使用缺省设备
			{
				alcGetDefaultDevice(AudioDeviceName);
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
			return (AL_FALSE);
		}
		#ifdef _DEBUG
		else
		{
			LOG_INFO(OS_TEXT("关联音频设备上下文与音频设备成功！"));
		}
		#endif//_DEBUG

		::strcpy(AudioDeviceName,alcGetString(AudioDevice,ALC_DEVICE_SPECIFIER));

		LOG_INFO("初始化音频设备完成: "+UTF8String(AudioDeviceName));

		LoadALFunc(AudioEM);

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

		LOG_INFO(OS_TEXT("初始化OpenAL完成！"));
		PutOpenALInfo();

		return(AL_TRUE);
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
			if(AudioDevice)	alcCloseDevice(AudioDevice);
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
//	void *alcGetCurrentDevice() { return (AudioDevice); }
//	char *alcGetCurrentDeviceName() { return(AudioDeviceName); }

	const struct Pair<ALenum,uint> al_format_bytes[]=
	{
		{AL_FORMAT_MONO8,			1},
		{AL_FORMAT_MONO16,			2},
		{AL_FORMAT_STEREO8,			2},
		{AL_FORMAT_STEREO16,		4},

// 		{AL_FORMAT_QUAD16,			8},
// 		{AL_FORMAT_51CHN16,			12},
// 		{AL_FORMAT_61CHN16,			14},
// 		{AL_FORMAT_71CHN16,			16},
//
// 		{AL_FORMAT_MONO_FLOAT32,	4},
// 		{AL_FORMAT_STEREO_FLOAT32,	8},
//
// 		{AL_FORMAT_QUAD8,			4},
// 		{AL_FORMAT_QUAD16,			8},
// 		{AL_FORMAT_QUAD32,			16},
// 		{AL_FORMAT_REAR8,			4},
// 		{AL_FORMAT_REAR16,			8},
// 		{AL_FORMAT_REAR32,			16},
// 		{AL_FORMAT_51CHN8,			6},
// 		{AL_FORMAT_51CHN16,			12},
// 		{AL_FORMAT_51CHN32,			24},
// 		{AL_FORMAT_61CHN8,			7},
// 		{AL_FORMAT_61CHN16,			14},
// 		{AL_FORMAT_61CHN32,			28},
// 		{AL_FORMAT_71CHN8,			8},
// 		{AL_FORMAT_71CHN16,			16},
// 		{AL_FORMAT_71CHN32,			32},

		{0,0}
	};

	const int al_get_format_byte(ALenum format)
	{
		const Pair<ALenum,uint> *p=al_format_bytes;

		while(p->first)
		{
			if(p->first==format)
				return p->second;
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
		ALuint uiSources[256];
		unsigned int iSourceCount = 0;

		// Clear AL Error Code
		alGetError();

		// Generate up to 256 Sources, checking for any errors
		for (iSourceCount = 0; iSourceCount < 256; iSourceCount++)
		{
			alGenSources(1, &uiSources[iSourceCount]);
			if (alGetError() != AL_NO_ERROR)
				break;
		}

		// Release the Sources
		alDeleteSources(iSourceCount, uiSources);
		if (alGetError() != AL_NO_ERROR)
		{
			for (unsigned int i = 0; i < 256; i++)
			{
				alDeleteSources(1, &uiSources[i]);
			}
		}

		return iSourceCount;
	}
	//--------------------------------------------------------------------------------------------------
	const char *alGetErrorInfo(const char *filename,const int line)
	{
		if(!alGetError)return(U8_TEXT("OpenALEE/OpenAL 未初始化!"));

		const char *result=nullptr;

		const char al_error_invalid				[]=U8_TEXT("invalid");
		const char al_error_invalid_name		[]=U8_TEXT("invalid name");
		const char al_error_invalid_enum		[]=U8_TEXT("invalid enum");
		const char al_error_invalid_value		[]=U8_TEXT("invalid value");
		const char al_error_invalid_operation	[]=U8_TEXT("invalid operation");
		const char al_error_out_of_memory		[]=U8_TEXT("out of memory");
		const char al_error_unknown_error		[]=U8_TEXT("unknown error");

		const ALenum error=alGetError();

		if(error==AL_NO_ERROR			)result=nullptr;else
		if(error==AL_INVALID			)result=al_error_invalid;else
		if(error==AL_INVALID_NAME		)result=al_error_invalid_name;else
		if(error==AL_INVALID_ENUM		)result=al_error_invalid_enum;else
		if(error==AL_INVALID_VALUE		)result=al_error_invalid_value;else
		if(error==AL_INVALID_OPERATION	)result=al_error_invalid_operation;else
		if(error==AL_OUT_OF_MEMORY		)result=al_error_out_of_memory;else
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
		LOG_INFO(OS_TEXT(			"　　　　　最大音源数: ")+OSString(GetMaxNumSources()));
		LOG_INFO(AudioEFX?OS_TEXT(	"　　　　　   EFX支持: 是")
						:OS_TEXT(	"　　　　　   EFX支持: 否"));
		LOG_INFO(AudioXRAM?OS_TEXT(	"           X-RAM支持: 是")
						:OS_TEXT(	"           X-RAM支持: 否"));

		if(AudioXRAM)
		{
			int size;

			alcGetIntegerv(AudioDevice,eXRAMSize,sizeof(int),&size);

			LOG_INFO(OS_TEXT("           X-RAM容量: ")+OSString(size));
		}

		char *al_ext;
		char *sp,*p,*p2;

		p=(char *)alGetString(AL_EXTENSIONS);
		p2=(char *)alcGetString(AudioDevice,ALC_EXTENSIONS);
		al_ext=new char[::strlen(p)+2+::strlen(p2)];
		::strcpy(al_ext,p);
		::strcat(al_ext," ");
		::strcat(al_ext,p2);

//		#ifdef _DEBUG
//		SaveMemoryToFile(u"OpenAL_Extensions.TXT",al_ext,::strlen(p)+2+::strlen(p2));
//		#endif//

		sp=al_ext;

		//有的声卡在两个扩展间用\n，有的用空格,所以做一下转换
		{
			p=sp;

			while(*p)
			{
				if(*p=='\n')
					*p=' ';

				p++;
			}
		}

		p=::strchr(sp,' ');
		if(p)
		{
			*p++=0;
			LOG_INFO(UTF8String(u8"　　支持的OpenAL扩展: ")+sp);

			sp=p;
			while(p=::strchr(sp,' '))
			{
				*p++=0;

                if(*sp)		//可能连续两个空格，没有信息
					LOG_INFO(UTF8String(u8"                      ")+sp);

				sp=p;
			}

			if(p>sp)
				LOG_INFO(UTF8String(u8"                      ")+sp);
		}
		else
		{
			LOG_INFO(UTF8String(u8"　　支持的OpenAL扩展: ")+al_ext);
		}

		delete[] al_ext;

		#ifdef _DEBUG
		LOG_INFO(OS_TEXT("输出OpenAL信息完成"));
		#endif//
	}

	/**
	* 初始化OpenAL
	* @param driver_name 驱动名称,如果不写则自动查找
	* @param device_name 设备名称,如果不写则自动选择缺省设备
	* @param enum_device 是否枚举所有的设备
	* @return 是否初始化成功
	*/
	bool InitOpenAL(const os_char *driver_name,const char *device_name,bool enum_device)
	{
		if (!InitOpenALDriver(driver_name))
			return(false);

		if (enum_device)
			EnumOpenALDevice();

		if (!InitOpenALDevice(device_name))
		{
			CloseOpenAL();
			return(false);
		}

		PutOpenALInfo();
		return(true);
	}
}//namespace openal
