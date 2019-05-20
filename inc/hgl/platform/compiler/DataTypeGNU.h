#ifndef HGL_DATATYPE_GNU_INCLUDE
#define HGL_DATATYPE_GNU_INCLUDE

namespace hgl
{
	using  int8     =  signed char;  	    ///<有符号 8位整型
	using uint8     =unsigned char;  	    ///<无符号 8位整型
	using  int16    =  signed short; 	    ///<有符号16位整型
	using uint16    =unsigned short; 	    ///<无符号16位整型
	using  int32    =  signed int; 	        ///<有符号32位整型
	using uint32    =unsigned int; 	        ///<无符号32位整型

	//64位系统下long/long long都是64位，32位系列下只有long long是64位

	using  int64    =  signed long long; 	///<有符号64位整型
	using uint64    =unsigned long long; 	///<无符号64位整型

    using float32   =float;
    using float64   =double;
}//namespace hgl
#endif//HGL_DATATYPE_GNU_INCLUDE
