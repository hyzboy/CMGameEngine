#ifndef HGL_ATOMIC_INCLUDE
#define HGL_ATOMIC_INCLUDE

#include<hgl/platform/Platform.h>

#if HGL_OS == HGL_OS_Windows
    #include<hgl/thread/atomic/AtomicWin.h>
#elif (HGL_OS == HGL_OS_macOS)||(HGL_OS == HGL_OS_Android)
    #include<atomic>

    template<typename T> using atom=std::atomic<T>;
#elif defined(__GNUC__)
    #if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 1) || __GNUC__ > 4) && (defined(__x86_64__) || defined(__i386__) || defined(__powerpc__))
        #include<hgl/thread/atomic/AtomicGNU.h>
    #else
        #include<hgl/thread/atomic/AtomicAPR.h>
    #endif//
#endif//

//ps.1：老旧的Linux/32bit下原子仅支持24位，但我们设定为不支持旧的Linux
//ps.2：使用GCC 4.1内置宏实现的AtomicGNU的不支持doubel型处理，如需支持，则尽可能不要用atom_double

//ps..........GCC4.7/4.8/4.9下如果使用c++11的atomic会造成一些valgrind-memcheck报错，所以暂不使用。待valgrind更新

namespace hgl
{
    #if (HGL_OS == HGL_OS_Windows)//&&(!defined(HGL_ATOMIC_CPP11))
        typedef atom_win32<int      > atom_int;
        typedef atom_win32<uint     > atom_uint;
        typedef atom_win32<int32    > atom_int32;
        typedef atom_win32<uint32   > atom_uint32;
        typedef atom_win64<int64    > atom_int64;
        typedef atom_win64<uint64   > atom_uint64;
        typedef atom_win32<float    > atom_float;
//      typedef atom_win64<double   > atom_double;

        typedef atom_win32<bool     > atom_bool;
        typedef atom_win32<char     > atom_char;
        typedef atom_win32<uchar    > atom_uchar;
//      typedef atom_win32<wchar_t  > atom_wchar;
        typedef atom_win32<u16char  > atom_char16;
//      typedef atom_win32<char32_t > atom_char32;
    #else
        typedef atom<bool       > atom_bool;
        typedef atom<int        > atom_int;
        typedef atom<uint       > atom_uint;
        typedef atom<int32      > atom_int32;
        typedef atom<uint32     > atom_uint32;
        typedef atom<int64      > atom_int64;
        typedef atom<uint64     > atom_uint64;
        typedef atom<float      > atom_float;
//      typedef atom<double     > atom_double;

        typedef atom<char       > atom_char;
        typedef atom<uchar      > atom_uchar;
//      typedef atom<wchar_t    > atom_wchar;
        typedef atom<u16char    > atom_char16;
//      typedef atom<char32_t   > atom_char32;
    #endif//windows & !c++11
}//namespace hgl
#endif//HGL_ATOMIC_INCLUDE
