#ifndef HGL_EXTERNAL_INCLUDE
#define HGL_EXTERNAL_INCLUDE

#include<hgl/external_core.h>

#if !defined(__MAKE_HGL__) && !defined(__MAKE_PLUGIN__)
	#pragma comment(lib,HGL_LIB_OpenALEE)		//连入OpenALEE音频库文件
	#pragma comment(lib,HGL_LIB_Graphics)		//连入图形库文件
	#pragma comment(lib,HGL_LIB_GUI)			//连入图形用户界面库文件
#endif//!__MAKE_HGL__&&!__MAKE_PLUGIN__
#endif//HGL_EXTERNAL_INCLUDE
