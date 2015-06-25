#ifndef HGL_COMPILER_GNU_INCLUDE
#define HGL_COMPILER_GNU_INCLUDE
//--------------------------------------------------------------------------------------------------
#define HGL_COMPILER_NAME		OS_TEXT("GNU C/C++")
#define HGL_LIB_COMPILER_NAME	OS_TEXT("GCC")

#if (__GNUC__<4)||(__GNUC__==4&&__GNUC_MINOR__<7)
	#error Please upgrade your compiler or development tools to GNU C/C++ 4.7 or later
#else

	#if __GNUC__ == 4
		#define HGL_LIB_COMPILER_MAIOR_VER  OS_TEXT("4")
	#elif __GNUC__ == 5
		#define HGL_LIB_COMPILER_MAIOR_VER  OS_TEXT("5")
	#elif __GNUC__ == 6
		#define HGL_LIB_COMPILER_MAIOR_VER  OS_TEXT("6")
	#elif __GNUC__ == 7
		#define HGL_LIB_COMPILER_MAIOR_VER  OS_TEXT("7")
    #elif __GNUC__ == 8
        #define HGL_LIB_COMPILER_MAIOR_VER  OS_TEXT("8")
    #elif __GNUC__ == 9
        #define HGL_LIB_COMPILER_MAIOR_VER  OS_TEXT("9")
    #elif __GNUC__ == 10
        #define HGL_LIB_COMPILER_MAIOR_VER  OS_TEXT("10")
	#endif//__GNUC__

	#if	__GNUC_MINOR__ == 0
		#define HGL_LIB_COMPILER_MINOR_VER	OS_TEXT("0")
	#elif __GNUC_MINOR__ == 1
		#define HGL_LIB_COMPILER_MINOR_VER	OS_TEXT("1")
	#elif __GNUC_MINOR__ == 2
		#define HGL_LIB_COMPILER_MINOR_VER	OS_TEXT("2")
	#elif __GNUC_MINOR__ == 3
		#define HGL_LIB_COMPILER_MINOR_VER	OS_TEXT("3")
	#elif __GNUC_MINOR__ == 4
		#define HGL_LIB_COMPILER_MINOR_VER	OS_TEXT("4")
	#elif __GNUC_MINOR__ == 5
		#define HGL_LIB_COMPILER_MINOR_VER	OS_TEXT("5")
	#elif __GNUC_MINOR__ == 6
		#define HGL_LIB_COMPILER_MINOR_VER	OS_TEXT("6")
	#elif __GNUC_MINOR__ == 7
		#define HGL_LIB_COMPILER_MINOR_VER	OS_TEXT("7")
	#elif __GNUC_MINOR__ == 8
		#define HGL_LIB_COMPILER_MINOR_VER	OS_TEXT("8")
	#elif __GNUC_MINOR__ == 9
		#define HGL_LIB_COMPILER_MINOR_VER	OS_TEXT("9")
	#endif//

	#if	__GNUC_PATCHLEVEL__ == 0
		#define HGL_LIB_COMPILER_PATCH_LEVEL	OS_TEXT("0")
	#elif __GNUC_PATCHLEVEL__ == 1
		#define HGL_LIB_COMPILER_PATCH_LEVEL	OS_TEXT("1")
	#elif __GNUC_PATCHLEVEL__ == 2
		#define HGL_LIB_COMPILER_PATCH_LEVEL	OS_TEXT("2")
	#elif __GNUC_PATCHLEVEL__ == 3
		#define HGL_LIB_COMPILER_PATCH_LEVEL	OS_TEXT("3")
	#elif __GNUC_PATCHLEVEL__ == 4
		#define HGL_LIB_COMPILER_PATCH_LEVEL	OS_TEXT("4")
	#elif __GNUC_PATCHLEVEL__ == 5
		#define HGL_LIB_COMPILER_PATCH_LEVEL	OS_TEXT("5")
	#elif __GNUC_PATCHLEVEL__ == 6
		#define HGL_LIB_COMPILER_PATCH_LEVEL	OS_TEXT("6")
	#elif __GNUC_PATCHLEVEL__ == 7
		#define HGL_LIB_COMPILER_PATCH_LEVEL	OS_TEXT("7")
	#elif __GNUC_PATCHLEVEL__ == 8
		#define HGL_LIB_COMPILER_PATCH_LEVEL	OS_TEXT("8")
	#elif __GNUC_PATCHLEVEL__ == 9
		#define HGL_LIB_COMPILER_PATCH_LEVEL	OS_TEXT("9")
	#endif//

	#define HGL_LIB_COMPILER_VERSION HGL_LIB_COMPILER_MAIOR_VER OS_TEXT(".") HGL_LIB_COMPILER_MINOR_VER OS_TEXT(".") HGL_LIB_COMPILER_PATCH_LEVEL

#endif//__GNUC__

//--------------------------------------------------------------------------------------------------
#if (__GNUC__<4)||(__GNUC__==4&&__GNUC_MINOR__<6)
	#define nullptr NULL
	#define enum_int(name)	enum name
	#define enum_uint(name)	enum name
#else
	#define enum_int(name)	enum name:int
	#define enum_uint(name)	enum name:uint
#endif//GCC 4.6

#ifdef HGL_CPP11
	#if (__GNUC__>4)||(__GNUC__==4&&__GNUC_MINOR__>6)
// 	#define HGL_ATOMIC_CPP11					//C++11原子模板
	#define HGL_VARIADIC_TEMPLATES				//变长参数模板
	#define HGL_INITIALIZER_LIST				//初始化列表
	#endif//GCC 4.7

	#if (__GNUC__<4)||(__GNUC__==4&&__GNUC_MINOR__<8)
		#define HGL_DEFAULT_MEMFUNC		{}
		#define HGL_OVERRIDE
	#else
		#define HGL_CONSTRUCTION_REUSE			//构造函数复用
		#define HGL_DEFAULT_MEMFUNC 	=default
		#define HGL_OVERRIDE			override
	#endif//GCC 4.8
#else
	#define HGL_DEFAULT_MEMFUNC			{}
	#define HGL_OVERRIDE
#endif//C++11

#define HGL_CPP11_INT128
#define HGL_FORCE_INLINE __inline __attribute__ ((always_inline))

#include<hgl/platform/compiler/DataTypeGNU.h>
#include<hgl/platform/compiler/Property.h>

#define HGL_THREAD		__thread
//--------------------------------------------------------------------------------------------------
#define HGL_LIB_FRONT	HGL_LIB_OS "_" HGL_LIB_COMPILER_NAME "_" HGL_LIB_DEBUG_NAME "_"

#define HGL_LIB_END		".a"
//--------------------------------------------------------------------------------------------------
#endif//HGL_COMPILER_GNU_INCLUDE
