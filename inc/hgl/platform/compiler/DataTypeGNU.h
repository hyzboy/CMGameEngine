#ifndef HGL_DATATYPE_GNU_INCLUDE
#define HGL_DATATYPE_GNU_INCLUDE
namespace hgl
{
	typedef   signed char        int8;  	///<有符号 8位整型
	typedef unsigned char       uint8;  	///<无符号 8位整型
	typedef   signed short       int16; 	///<有符号16位整型
	typedef unsigned short      uint16; 	///<无符号16位整型
	typedef   signed int         int32; 	///<有符号32位整型
	typedef unsigned int        uint32; 	///<无符号32位整型

	//64位系统下long/long long都是64位，32位系列下只有long long是64位

	typedef   signed long long 	 int64; 	///<有符号64位整型
	typedef unsigned long long 	uint64; 	///<无符号64位整型
}//namespace hgl
#endif//HGL_DATATYPE_GNU_INCLUDE
