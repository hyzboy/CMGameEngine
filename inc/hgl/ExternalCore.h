#ifndef HGL_EXTERNAL_CORE_INCLUDE
#define HGL_EXTERNAL_CORE_INCLUDE

#include<hgl/platform/platform.h>
#include<hgl/platform/ConsoleSystemInfo.h>
#include<hgl.h>

#if !defined(__MAKE_HGL__) && !defined(__MAKE_PLUGIN__)
	#pragma comment(lib,HGL_LIB_Base)         	//连入基础库文件
	#pragma comment(lib,HGL_LIB_APR)			//连入Apache Portable Runtime
	#pragma comment(lib,HGL_LIB_Network)      	//连入网络库文件
	#pragma comment(lib,HGL_LIB_UtilityTools) 	//连入实用工具库文件
	#pragma comment(lib,HGL_LIB_WinExternal)	//连入标准Windows平台接口库文件
#endif//!__MAKE_HGL__&&!__MAKE_PLUGIN__

namespace hgl
{
	bool InitCore(ConsoleSystemInfo &si,bool create_log);	///<初始化核心
	void CloseCore();										///<关闭核心
}//namespace hgl
#endif//HGL_EXTERNAL_CORE_INCLUDE
