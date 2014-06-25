#ifndef HGL_COMPILER_INTEL_INCLUDE
#define HGL_COMPILER_INTEL_INCLUDE
//--------------------------------------------------------------------------------------------------
#define HGL_COMPILER_NAME	u"Intel C/C++"
//--------------------------------------------------------------------------------------------------
#if __INTEL_COMPILER < 1200
	#error Please upgrade your compiler To Intel C/C++ 12.1 or later.
#else
	#if __INTEL_COMPILER >=1200 && __INTEL_COMPILER < 1300
		#define HGL_LIB_COMPILER_NAME "IntelCPP12"
	#else
		#define HGL_LIB_COMPILER_NAME "IntelCPP_UnknownVersion"
	#endif//__INTEL_COMPILER

	#define enum_uint(name)	enum name:unsigned int
#endif//__INTEL_COMPILER

#define vsnwprintf _vsnwprintf
//--------------------------------------------------------------------------------------------------
#include<hgl/platform/compiler/DataTypeTypedef.h>
#include<hgl/platform/compiler/Property.h>

#if HGL_OS == HGL_OS_Windows
	#define HGL_THREAD		__declspec(thread)
#else
	#define HGL_THREAD		__thread
#endif//HGL_OS == HGL_OS_Windows
//--------------------------------------------------------------------------------------------------
#ifdef _DLL
	#define HGL_LIB_CRT	"Dynamic"
#else
	#define HGL_LIB_CRT	"Static"
#endif//_DLL

#define HGL_LIB_FRONT	HGL_LIB_OS "_" HGL_LIB_COMPILER_NAME "_" HGL_LIB_DEBUG_NAME "_" HGL_LIB_CRT "_"

#if HGL_OS == HGL_OS_Windows
	#define HGL_LIB_END		".LIB"
#elif
	#define HGL_LIB_END		".a"
#endif//HGL_OS == HGL_OS_Windows
#endif//HGL_COMPILER_INTEL_INCLUDE
