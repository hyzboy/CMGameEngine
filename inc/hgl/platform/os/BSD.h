#ifndef HGL_OS_BSD_INCLUDE
#define HGL_OS_BSD_INCLUDE

#include<limits.h>
//--------------------------------------------------------------------------------------------------
using u32char           =char32_t;
using u16char           =char16_t;
using os_char           =char;
#define to_oschar       to_u8
#define OS_TEXT(str)    u8##str
#define U8_TEXT(str)    u8##str
#define U16_TEXT(str)   u##str

#if defined(__FreeBSD__)||defined(__FreeBSD)
    #define HGL_OS_NAME         OS_TEXT("FreeBSD")
    #define HGL_LIB_OS_NAME     "FreeBSD"
#elif defined(__NetBSD__)||defined(__NetBSD)
    #define HGL_OS_NAME         OS_TEXT("NetBSD")
    #define HGL_LIB_OS_NAME     "NetBSD"
#elif defined(__OpenBSD__)||defined(__OPENBSD)
    #define HGL_OS_NAME         OS_TEXT("OpenBSD")
    #define HGL_LIB_OS_NAME     "OpenBSD"
#else
    #define HGL_OS_NAME         OS_TEXT("BSD")
    #define HGL_LIB_OS_NAME     "BSD"
#endif//

#define HGL_LIB_OS              "BSD"                                           //库操作系统前缀
//--------------------------------------------------------------------------------------------------
#define HGL_PLUGIN_FRONTNAME    "libCMP."                                       //插件文件名前缀
#define HGL_PLUGIN_EXTNAME      ".so"                                           //插件文件扩展名
#define HGL_PLUGIN_FUNC         extern "C"                                      //插件函数定义

#define HGL_DIRECTORY_SEPARATOR     '/'                                         //目录分隔符
#define HGL_DIRECTORY_SEPARATOR_STR OS_TEXT("/")                                //目录分隔符
#define HGL_DIRECTORY_SEPARATOR_U8STR U8_TEXT("/")                              //目录分隔符

#define HGL_LINE_END            "\n"                                            //换行符
#define HGL_LINE_END_SIZE       1                                               //换行符长度

#define HGL_MEM_ALIGN           16                                              //内存对齐字节数

#define HGL_MAX_PATH            _POSIX_PATH_MAX

#define HGL_FMT_I64             "%lld"
//--------------------------------------------------------------------------------------------------
#include<stdlib.h>
#include<hgl/platform/os/PosixThread.h>

#define hgl_malloc(size)        aligned_alloc(HGL_MEM_ALIGN,size)         //这个是C11新增，需要libc 2.16
#define hgl_realloc             realloc
#define hgl_free                free

template<typename T>
inline T *hgl_aligned_malloc(size_t n)
{
    return (T *)aligned_alloc(alignof(T),n*sizeof(T));
}

#define OS_EXTERNAL_H           <dlfcn.h>
typedef void *                  ExternalModulePointer;
#define pi_get                  dlsym
#define pi_close                dlclose

typedef pthread_mutex_t hgl_thread_mutex;

#define hgl_stat64              stat
#define hgl_open64              open
#define hgl_lseek64             lseek
#define hgl_tell64(fp)          lseek(fp,0,SEEK_CUR)
#define hgl_fstat64             fstat
#define hgl_lstat64             lstat
#define hgl_read64              read
#define hgl_write64             write
#define hgl_pread64             pread
#define hgl_pwrite64            pwrite

#define struct_stat64           struct stat
#define struct_dirent64         struct dirent
#define hgl_dirent64            dirent
#define hgl_readdir64           readdir
//--------------------------------------------------------------------------------------------------
#endif//HGL_OS_BSD_INCLUDE
