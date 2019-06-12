#ifndef HGL_COMPILER_LLVM_CLANG_INCLUDE
#define HGL_COMPILER_LLVM_CLANG_INCLUDE
//--------------------------------------------------------------------------------------------------
#define HGL_COMPILER_NAME       OS_TEXT("LLVM Clang")
#define HGL_LIB_COMPILER_NAME   OS_TEXT("LLVM")

#if (__clang_major__<3)||(__clang_major__==3&&__clang_minor__<7)
    #error Please upgrade your compiler or development tools to LLVM Clang 3.7 or later
#else

    #define HGL_LIB_COMPILER_VERSION __clang_version__

#endif//__clang__

#define HGL_THREAD_LOCAL_STORAGE        __thread                                //线程本地储存
//--------------------------------------------------------------------------------------------------
#define HGL_LIB_FRONT   HGL_LIB_OS "_" HGL_LIB_COMPILER_NAME "_" HGL_LIB_DEBUG_NAME "_"

#define HGL_LIB_END     ".a"

#include<hgl/platform/compiler/DataTypeGNU.h>
#include<hgl/platform/compiler/DataTypeTiny.h>
#include<hgl/platform/compiler/Property.h>
//--------------------------------------------------------------------------------------------------
#endif//HGL_COMPILER_LLVM_CLANG_INCLUDE
