#ifndef HGL_COMPILER_WATCOM_INCLUDE
#define HGL_COMPILER_WATCOM_INCLUDE
//--------------------------------------------------------------------------------------------------   
#define HGL_COMPILER_NAME	u"OpenWatcom C/C++"

#if __WATCOMC__ < 1270			//最低OpenWatcom 1.7
	#if __WATCOMC__ < 1200
		#error Please upgrade your compiler or development tools to OpenWatcom C/C++ 1.7 or later.
	#else
		#error  please upgrade to OpenWatcom C/C++ 1.7 or later.
	#endif//
#endif//

#define enum_uint(name)	enum name
//--------------------------------------------------------------------------------------------------
namespace hgl
{
	typedef unsigned char       uchar;  	///< 无符号字符型
	typedef unsigned int        uint;   	///< 无符号整型

	typedef   signed char        int8;  	///<有符号 8位整型
	typedef unsigned char       uint8;  	///<无符号 8位整型
	typedef   signed short       int16; 	///<有符号16位整型
	typedef unsigned short      uint16; 	///<无符号16位整型
	typedef   signed long        int32; 	///<有符号32位整型
	typedef unsigned long       uint32; 	///<无符号32位整型
	typedef   signed __int64   	 int64; 	///<有符号64位整型
	typedef unsigned __int64    uint64; 	///<无符号64位整型
}
//--------------------------------------------------------------------------------------------------
#include<hgl/platform/compiler/Property.h>
//--------------------------------------------------------------------------------------------------
#endif//HGL_COMPILER_WATCOM_INCLUDE

