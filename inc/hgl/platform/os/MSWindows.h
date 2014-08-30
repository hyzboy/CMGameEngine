#ifndef HGL_OS_WIN_INCLUDE
#define HGL_OS_WIN_INCLUDE

#include<malloc.h>
#include<windows.h>
//--------------------------------------------------------------------------------------------------
#define u16char			wchar_t
#define os_char			wchar_t
#define to_oschar		to_u16
#define OS_TEXT(str)	L##str
#define U8_TEXT(str)	str
#define U16_TEXT(str)	L##str

#define HGL_OS_NAME				OS_TEXT("Windows")

#if HGL_CPU == HGL_CPU_X86_32
	#if (!defined(_WIN32_WINNT))||(_WIN32_WINNT<0x0501)
		#undef _WIN32_WINNT
		#define _WIN32_WINNT 0x0501			//声明可以使用Windows XP级别的Windows API
	#endif//_WIN32_WINNT

	#define HGL_LIB_OS					"Win32"											//库操作系统前缀

#elif HGL_CPU == HGL_CPU_X86_64
	#if (!defined(_WIN32_WINNT))||(_WIN32_WINNT<0x0502)
		#undef _WIN32_WINNT
		#define _WIN32_WINNT 0x0502			//声明可以使用Windows Server 2003/XP-64级别的Windows API
	#endif//_WIN32_WINNT

	#define HGL_LIB_OS					"Win64"											//库操作系统前缀
#endif//HGL_CPU
//--------------------------------------------------------------------------------------------------
#define HGL_PLUGIN_FRONTNAME			OS_TEXT("CMP.")									//插件文件名前缀
#define HGL_PLUGIN_EXTNAME	    		OS_TEXT(".DLL")									//插件文件扩展名
#define HGL_PLUGIN_FUNC					extern "C" __declspec(dllexport)				//插件函数定义

#define HGL_DIRECTORY_SEPARATOR 		OS_TEXT('\\')									//目录分隔符
#define HGL_LINE_END					OS_TEXT("\n\r")									//换行符
#define HGL_LINE_END_SIZE				2												//换行符长度

#define HGL_MAX_PATH					MAX_PATH

#define HGL_MEM_ALIGN					16												//内存对齐字节数

#define HGL_GL_WINDOW_INCLUDE_FILE		<hgl/platform/WinOpenGL.h>						//指定OpenGL窗口引用头文件
#define HGL_GL_WINDOW_CLASS				WinGLWindow										//指定OpenGL窗口类名称
//--------------------------------------------------------------------------------------------------
#if HGL_COMPILER == HGL_COMPILER_Microsoft
    #define hgl_malloc(size)        _aligned_malloc(size,HGL_MEM_ALIGN)
    #define hgl_realloc(ptr,size)   _aligned_realloc(ptr,size,HGL_MEM_ALIGN)
    #define hgl_free                _aligned_free
#else
    #define hgl_malloc(size)        memalign(HGL_MEM_ALIGN,size)
    #define hgl_realloc(ptr,size)   realloc(ptr,size)
    #define hgl_free                free
#endif//

typedef	CRITICAL_SECTION		hgl_thread_mutex;

#define struct_stat64			struct _stat64
#define stat64					_stat64
#define lseek64					_lseeki64
#define tell64(fp)				_telli64(fp)
#define fstat64					_fstati64
#define lstat64					_wstat64
#define read64					_read
#define write64					_write
//--------------------------------------------------------------------------------------------------
#endif//HGL_OS_WIN_INCLUDE
