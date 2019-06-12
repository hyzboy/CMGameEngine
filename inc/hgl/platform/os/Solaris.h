#ifndef HGL_OS_Solaris_INCLUDE
#define HGL_OS_Solaris_INCLUDE
//--------------------------------------------------------------------------------------------------
#if HGL_CPU == HGL_CPU_X86_32
    #define HGL_OS_NAME             u"Solaris(x86-32)"
#elif HGL_CPU == HGL_CPU_X86_64
    #define HGL_OS_NAME             u"Solaris(x86-64)"
#elif HGL_CPU == HGL_CPU_Sparc
    #define HGL_OS_NAME             u"Solaris(Sparc)"
#else
    #error 未知类型 Solaris
#endif//
//--------------------------------------------------------------------------------------------------
#define HGL_PLUGIN_EXTNAME      u".so"                                          //插件文件扩展名
#define HGL_PLUGIN_FUNC

#define HGL_DIRECTORY_SEPARATOR L'/'                                            //目录分隔符
#define HGL_LINE_END            u"\n"                                           //换行符
#define HGL_LINE_END_SIZE       1                                               //换行符长度

#define HGL_MEM_ALIGN           16                                              //内存对齐字节数

#define HGL_MAX_PATH            256
//--------------------------------------------------------------------------------------------------
#include<stddef.h>
#include<malloc.h>
#include<pthread.h>

#define hgl_malloc(size)        malloc(size)
#define hgl_realloc(ptr,size)   realloc(ptr,size)
#define hgl_free                free

typedef pthread_mutex_t hgl_thread_mutex;

#define iconv_str               const char *

#define struct_stat64           struct stat64
#define read64                  read
#define write64                 write

#define struct_dirent64         struct dirent64
//--------------------------------------------------------------------------------------------------
#endif//HGL_OS_Solaris_INCLUDE
