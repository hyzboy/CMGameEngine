#ifndef HGL_COMPILER_CPlusPlusBuilder_INCLUDE
#define HGL_COMPILER_CPlusPlusBuilder_INCLUDE
//--------------------------------------------------------------------------------------------------
#if __BORLANDC__ < 0x0610			//0x610 C++Builder 2009
	#error Borland legend does not exist, please upgrade your compiler or development tools to C++Builder 2009 or later.	//不支持更低版本的编译器
#else
	#define enum_uint(name)	enum name:unsigned int

	#if __BORLANDC__< 0x620
		#define HGL_COMPILER_NAME		u"CodeGear C++"
	#else
		#define HGL_COMPILER_NAME		u"Embarcadero C++"
	#endif//

	#if __BORLANDC__ >= 0x0670
		#define HGL_LIB_COMPILER_NAME		"CB_UnknownVersion"
		#define HGL_LIB_COMPILER_VERSION	u"Unknown"
	#elif __BORLANDC__ >= 0x0660
		#define HGL_LIB_COMPILER_NAME		"CBXE4"
		#define HGL_LIB_COMPILER_VERSION	u"6.6"
	#elif __BORLANDC__ >= 0x0650
		#define HGL_LIB_COMPILER_NAME		"CBXE3"
		#define HGL_LIB_COMPILER_VERSION	u"6.5"
	#elif __BORLANDC__ >= 0x0640
		#define HGL_LIB_COMPILER_NAME		"CBXE2"
	#define HGL_LIB_COMPILER_VERSION		u"6.4"
	#elif __BORLANDC__ >= 0x0630
		#define HGL_LIB_COMPILER_NAME		"CBXE"
		#define HGL_LIB_COMPILER_VERSION	u"6.3"
	#elif __BORLANDC__ >= 0x0620
		#define HGL_LIB_COMPILER_NAME		"CB2010"
		#define HGL_LIB_COMPILER_VERSION	u"6.2"
	#elif __BORLANDC__ >= 0x0610
		#define HGL_LIB_COMPILER_NAME		"CB2009"
		#define HGL_LIB_COMPILER_VERSION	u"6.1"
	#endif//
#endif//__BORLANDC__

#pragma option -K                       //强制char数据类型为无符号型
#pragma option -6                       //按Pentium Pro/PentiumII方式优化
#pragma option -b                       //强制将Enum转为int
//--------------------------------------------------------------------------------------------------
#include<hgl/platform/compiler/DataTypeTypedef.h>
#include<hgl/platform/compiler/Property.h>

#define HGL_THREAD		__declspec(thread)

#define HGL_LIB_FRONT	HGL_LIB_OS "_" HGL_LIB_COMPILER_NAME "_" HGL_LIB_DEBUG_NAME "_"

#define HGL_LIB_END		".LIB"
//--------------------------------------------------------------------------------------------------
#endif//HGL_COMPILER_CPlusPlusBuilder_INCLUDE

