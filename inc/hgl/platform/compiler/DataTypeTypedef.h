#ifndef HGL_DATATYPE_TYPEDEF_INCLUDE
#define HGL_DATATYPE_TYPEDEF_INCLUDE
namespace hgl
{
	using  int8     =  signed __int8 ;	///<有符号 8位整型
	using uint8     =unsigned __int8 ;	///<无符号 8位整型
	using  int16    =  signed __int16;	///<有符号16位整型
	using uint16    =unsigned __int16;	///<无符号16位整型
	using  int32    =  signed __int32;	///<有符号32位整型
	using uint32    =unsigned __int32;	///<无符号32位整型
	using  int64    =  signed __int64;	///<有符号64位整型
	using uint64    =unsigned __int64;	///<无符号64位整型

    using float32   =float;
    using float64   =double;
    using float128  =long double;       ///<Visual C++ 2017下long double实质依然精度等于double。x86-64 CPU已有实现128位浮点计算，可使用外联汇编方式实现。
}//namespace hgl
#endif//HGL_DATATYPE_TYPEDEF_INCLUDE
