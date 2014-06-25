//--------------------------------------------------------------------------------------------------
// OpenAL ExtEdition SDK
//
// 这是一个根据标准OpenAL SDK所编写的特殊增强版本，它支持目前所有的OpenAL扩展。
//
//           作者: 胡颖卓
//           版本: 1.15
// 对应OpenAL版本: 1.1
//       对应语言: C/C++
//		对应操作系统: Windows,Linux
//   适用开发工具: Borland C++ Compiler 5.5 或更高
//                 Borland C++ Builder 5 或更高
//                 Borland C++ BuilderX 1.0 或更高
//				   Microsoft C++ 15.0 (Visual C++ 9.0)或更高
//				   GNU C/C++ 4.0 或更高
//
// 第一版编写时间: 2003年10月5日
// 当前版完成时间: 2014年4月9日
//
//       官方网站: http://www.hyzgame.com.cn/openalee
// OpenAL官方网站: http://www.openal.org
//--------------------------------------------------------------------------------------------------
#ifndef OpenALH
#define OpenALH

#ifdef __al_h_
	#error 请不要使用原始的OpenAL头文件
#else
	#define __al_h_
#endif //__al_h_

#include<hgl/platform/Platform.h>

#include<hgl/al/al.h>
#include<hgl/al/alc.h>
#include<hgl/al/xram.h>
#include<hgl/al/efx.h>

namespace openal                                                									///OpenAL EE所使用的名字空间
{
	bool LoadOpenAL(const os_char *driver_name=nullptr);											///<加载OpenAL,参数为驱动名称

	const char *alcGetDeviceList();																	///<取得设备列表
	bool alcGetDefaultDevice(char *);																///<取得缺省设备
	void alcSetDefaultContext();																	///<设置缺省上下文

	const os_char *alGetErrorInfo(const char *,const int);											///<取得最近的错误

	/**
	 * 初始化OpenAL驱动
	 * @param driver_name 驱动名称
	 * @return 是否成功
	 */
	bool InitOpenALDriver(const os_char *driver_name=nullptr);										///<初始化OpenAL驱动

	/**
	 * 初始化OpenAL设备
	 * @param device_name 设备名称
	 * @return 是否成功
	 */
	bool InitOpenALDevice(const char *device_name=nullptr);
	void CloseOpenAL();                                                                             ///<关闭OpenAL

	unsigned int AudioTime(ALenum,ALsizei);
	double AudioDataTime(ALuint,ALenum,ALsizei);
	//--------------------------------------------------------------------------------------------------
	#define alLastError()   alGetErrorInfo(__FILE__,__LINE__)
}//namespace openal
#endif //OpenALH
