#ifndef HGL_COMPILER_LLVM_CLANG_INCLUDE
#define HGL_COMPILER_LLVM_CLANG_INCLUDE
//--------------------------------------------------------------------------------------------------
#define HGL_COMPILER_NAME		OS_TEXT("LLVM Clang")
#define HGL_LIB_COMPILER_NAME	OS_TEXT("LLVM")

#if __clang_major__ < 3
	#error Please upgrade your compiler or development tools to LLVM Clang 3.0 or later
#else

	#if __clang_major__ == 3
		#define HGL_LIB_COMPILER_MAIOR_VER	OS_TEXT("3")
	#elif __clang_major__ == 4
		#define HGL_LIB_COMPILER_MAIOR_VER	OS_TEXT("4")
	#elif __clang_major__ == 5
		#define HGL_LIB_COMPILER_MAIOR_VER	OS_TEXT("5")
	#elif __clang_major__ == 6
		#define HGL_LIB_COMPILER_MAIOR_VER	OS_TEXT("6")
	#elif __clang_major__ == 7
		#define HGL_LIB_COMPILER_MAIOR_VER	OS_TEXT("7")
	#elif __clang_major__ == 8
		#define HGL_LIB_COMPILER_MAIOR_VER	OS_TEXT("8")
	#elif __clang_major__ == 9
		#define HGL_LIB_COMPILER_MAIOR_VER	OS_TEXT("9")
	#elif __clang_major__ == 10
		#define HGL_LIB_COMPILER_MAIOR_VER	OS_TEXT("10")
	#endif//__clang_major__

	#if	__clang_minor__ == 0
		#define HGL_LIB_COMPILER_MINOR_VER	OS_TEXT("0")
	#elif __clang_minor__ == 1
		#define HGL_LIB_COMPILER_MINOR_VER	OS_TEXT("1")
	#elif __clang_minor__ == 2
		#define HGL_LIB_COMPILER_MINOR_VER	OS_TEXT("2")
	#elif __clang_minor__ == 3
		#define HGL_LIB_COMPILER_MINOR_VER	OS_TEXT("3")
	#elif __clang_minor__ == 4
		#define HGL_LIB_COMPILER_MINOR_VER	OS_TEXT("4")
	#elif __clang_minor__ == 5
		#define HGL_LIB_COMPILER_MINOR_VER	OS_TEXT("5")
	#elif __clang_minor__ == 6
		#define HGL_LIB_COMPILER_MINOR_VER	OS_TEXT("6")
	#elif __clang_minor__ == 7
		#define HGL_LIB_COMPILER_MINOR_VER	OS_TEXT("7")
	#elif __clang_minor__ == 8
		#define HGL_LIB_COMPILER_MINOR_VER	OS_TEXT("8")
	#elif __clang_minor__ == 9
		#define HGL_LIB_COMPILER_MINOR_VER	OS_TEXT("9")
	#endif//

	#define HGL_LIB_COMPILER_VERSION HGL_LIB_COMPILER_MAIOR_VER OS_TEXT(".") HGL_LIB_COMPILER_MINOR_VER

#endif//__clang__
//--------------------------------------------------------------------------------------------------
#include<hgl/platform/compiler/DataTypeGNU.h>
#include<hgl/platform/compiler/Property.h>

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
// 	#define HGL_VARIADIC_TEMPLATES				//变长参数模板	clang 3.4不支持
	#define HGL_INITIALIZER_LIST				//初始化列表
#else
	#define HGL_DEFAULT_MEMFUNC		{}
	#define HGL_OVERRIDE
#endif//clang >=3.4
//--------------------------------------------------------------------------------------------------
#define HGL_LIB_FRONT	HGL_LIB_OS "_" HGL_LIB_COMPILER_NAME "_" HGL_LIB_DEBUG_NAME "_"

#define HGL_LIB_END		".a"
//--------------------------------------------------------------------------------------------------
#endif//HGL_COMPILER_LLVM_CLANG_INCLUDE
