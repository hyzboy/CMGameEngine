#ifndef HGL_COMPILER_MICROSOFT_INCLUDE
#define HGL_COMPILER_MICROSOFT_INCLUDE
//--------------------------------------------------------------------------------------------------
#define HGL_COMPILER_NAME            OS_TEXT("Microsoft C/C++")
#define HGL_LIB_COMPILER_NAME        OS_TEXT("MSC")

#if _MSC_VER < 1900                            //Visual C++ 2015(19)
    #error Please upgrade your compiler or development tools to Microsoft C/C++ 19.0 (Visual C++ 2015) or later.
#else
    #if _MSC_VER >= 1910
        #define HGL_LIB_COMPILER_VERSION    OS_TEXT("19.1")      //Visual C++ 2017
    #elif _MSC_VER == 1900
        #define HGL_LIB_COMPILER_VERSION    OS_TEXT("19")        //Visual C++ 2015
    #else
        #define HGL_LIB_COMPILER_VERSION    OS_TEXT("Unknow")
    #endif//_MSC_VER
#endif//_MSC_VER

#define HGL_THREAD_LOCAL_STORAGE        __declspec(thread)                              //线程本地储存
//--------------------------------------------------------------------------------------------------
#define HGL_FMT_I64            "%I64d"
#define HGL_FMT_U64            "%I64u"
//参考文档最后查阅支持版本为VC2013，网址：http://msdn.microsoft.com/en-us/library/tcxf1dw6.aspx
//--------------------------------------------------------------------------------------------------
#define _USE_MATH_DEFINES                // 使用数学常数定义
//--------------------------------------------------------------------------------------------------
#pragma warning(disable:4819)            // ansi -> unicode
#pragma warning(disable:4311)            // 模板警告
#pragma warning(disable:4800)            // -> bool 性能损失警告
#pragma warning(disable:4244)            // -> int 精度丢失警告
#pragma warning(disable:4804)            // 不安全的类型比较
#pragma warning(disable:4805)            // 不安全的类型比较

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif//

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif//

#ifndef _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif//

#ifdef _DLL
    #define HGL_LIB_CRT    "Dynamic"
#else
    #define HGL_LIB_CRT    "Static"
#endif//_DLL

#define HGL_LIB_FRONT    HGL_LIB_OS "_" HGL_LIB_COMPILER_NAME "_" HGL_LIB_DEBUG_NAME "_" HGL_LIB_CRT "_"

#define HGL_LIB_END        ".LIB"
//--------------------------------------------------------------------------------------------------
#include<hgl/platform/compiler/DataTypeWin.h>
#include<hgl/platform/compiler/DataTypeTiny.h>
#include<hgl/platform/compiler/Property.h>
//--------------------------------------------------------------------------------------------------
#endif//HGL_COMPILER_MICROSOFT_INCLUDE

