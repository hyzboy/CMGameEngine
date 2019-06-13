#ifndef HGL_OS_CYGWIN_INCLUDE
#define HGL_OS_CYGWIN_INCLUDE
//--------------------------------------------------------------------------------------------------
typedef char            os_char;
#define to_oschar       to_u8
#define OS_TEXT(str)    u8##str
#define U16_TEXT(str)   u16##str

#define HGL_OS_NAME             OS_TEXT("Cygwin")
#define HGL_LIB_OS_NAME         "Cygwin"

#if HGL_CPU == HGL_CPU_X86_32
    #if (!defined(_WIN32_WINNT))||(_WIN32_WINNT<0x0501)
        #undef _WIN32_WINNT
        #define _WIN32_WINNT 0x0501         //声明可以使用Windows XP级别的Windows API
    #endif//_WIN32_WINNT

    #define HGL_LIB_OS                  "Cygwin32"                                      //库操作系统前缀

#elif HGL_CPU == HGL_CPU_X86_64
    #if (!defined(_WIN32_WINNT))||(_WIN32_WINNT<0x0502)
        #undef _WIN32_WINNT
        #define _WIN32_WINNT 0x0502         //声明可以使用Windows Server 2003/XP-64级别的Windows API
    #endif//_WIN32_WINNT

    #define HGL_LIB_OS                  "Cygwin64"                                      //库操作系统前缀
#endif//HGL_CPU

#pragma comment(lib,"libws2_32.a")          //Windows Socket 2.0
//--------------------------------------------------------------------------------------------------
#define HGL_PLUGIN_EXTNAME              ".dll"                                          //插件文件扩展名
#define HGL_PLUGIN_FUNC                 extern "C" __declspec(dllexport)                //插件函数定义

#define HGL_DIRECTORY_SEPARATOR         '\\'                                            //目录分隔符
#define HGL_LINE_END                    "\n\r"                                          //换行符
#define HGL_LINE_END_SIZE               2                                               //换行符长度

#define HGL_MAX_PATH                    256

#define HGL_MEM_ALIGN                   16                                              //内存对齐字节数

#define HGL_GL_WINDOW_INCLUDE_FILE      <hgl/platform/WinOpenGL.h>                      //指定OpenGL窗口引用头文件
#define HGL_GL_WINDOW_CLASS             WinGLWindow                                     //指定OpenGL窗口类名称
//--------------------------------------------------------------------------------------------------
#include<malloc.h>
#include<pthread.h>

#define hgl_malloc(size)        memalign(HGL_MEM_ALIGN,size)
#define hgl_realloc(ptr,size)   realloc(ptr,size)
#define hgl_free                free

typedef pthread_mutex_t hgl_thread_mutex;

#define iconv_str               char *

#define struct_stat64           struct stat
#define fstat64                 fstat
#define lstat64                 lstat

#define struct_dirent64         struct dirent
#define readdir64               readdir

#define open64                  open
#define lseek64                 lseek
#define tell64(fp)              lseek(fp,0,SEEK_CUR)
#define read64                  read
#define write64                 write
#define pread64                 pread
#define pwrite64                pwrite
//--------------------------------------------------------------------------------------------------
#endif//HGL_OS_CYGWIN_INCLUDE
