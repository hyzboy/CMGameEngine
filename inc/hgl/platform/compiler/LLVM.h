#ifndef HGL_COMPILER_LLVM_CLANG_INCLUDE
#define HGL_COMPILER_LLVM_CLANG_INCLUDE
//--------------------------------------------------------------------------------------------------
#define HGL_COMPILER_NAME		OS_TEXT("LLVM Clang")
#define HGL_LIB_COMPILER_NAME	OS_TEXT("LLVM")

#if __clang_major__ < 3
	#error Please upgrade your compiler or development tools to LLVM Clang 3.0 or later
#else

	#define HGL_LIB_COMPILER_VERSION __clang_version__

#endif//__clang__
//--------------------------------------------------------------------------------------------------
// #if (__clang_major__>=3)||(__clang_major__==3&&__clang_minor__>=1)
// 	#define HGL_ATOMIC_CPP11					//C++11原子模板
// #endif//clang >=3.1

#define HGL_THREAD				__thread
#define enum_int(name)			enum name:int
#define enum_uint(name)			enum name:uint

#if (__clang_major__>=3)||(__clang_major__==3&&__clang_minor__>=4)
	#define HGL_CONSTRUCTION_REUSE				//构造函数复用
	#define HGL_DEFAULT_MEMFUNC		=default
	#define HGL_OVERRIDE			override
	#define HGL_INITIALIZER_LIST				//初始化列表

    #if (__clang_major__>=3)||(__clang_major__==3&&__clang_minor__>=5)
        #define HGL_VARIADIC_TEMPLATES              //变长参数模板
    #endif//Clang >=3.5
#else
	#define HGL_DEFAULT_MEMFUNC		{}
	#define HGL_OVERRIDE
#endif//clang >=3.4
//--------------------------------------------------------------------------------------------------
#define HGL_LIB_FRONT	HGL_LIB_OS "_" HGL_LIB_COMPILER_NAME "_" HGL_LIB_DEBUG_NAME "_"

#define HGL_LIB_END		".a"

#include<hgl/platform/compiler/DataTypeGNU.h>
#include<hgl/platform/compiler/Property.h>
//--------------------------------------------------------------------------------------------------
#endif//HGL_COMPILER_LLVM_CLANG_INCLUDE
