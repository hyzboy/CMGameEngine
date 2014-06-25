#ifndef HGL_COMPILER_IBM_INCLUDE
#define HGL_COMPILER_IBM_INCLUDE
//--------------------------------------------------------------------------------------------------
#if __IBMCPP__ >= 800
	#define HGL_COMPILER_NAME		u"XL C/C++"
#else
	#define HGL_COMPILER_NAME		u"VisualAge C/C++"
#endif//__IBMCPP__ >= 800

#define HGL_LIB_COMPILER_NAME	"8"
#define HGL_COMPILER_VERSION	u"8"

#define enum_uint(name)	enum name
//--------------------------------------------------------------------------------------------------
#include<hgl/platform/compiler/DataTypeGNU.h>
#include<hgl/platform/compiler/Property.h>
#include<malloc.h>
//--------------------------------------------------------------------------------------------------
#define HGL_LIB_FRONT	HGL_LIB_OS "_" HGL_LIB_COMPILER_NAME "_" HGL_LIB_DEBUG_NAME "_"

#define HGL_LIB_END		".LIB"
//--------------------------------------------------------------------------------------------------
#endif//HGL_COMPILER_IBM_INCLUDE
