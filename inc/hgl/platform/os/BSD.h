#ifndef HGL_OS_BSD_INCLUDE
#define HGL_OS_BSD_INCLUDE
//--------------------------------------------------------------------------------------------------
typedef char			os_char;
#define to_oschar		to_u8
#define OS_TEXT(str)	u8##str
#define U16_TEXT(str)	u16##str

#if defined(__FreeBSD__)||defined(__FreeBSD)
	#define HGL_OS_NAME			u"FreeBSD"
	#define HGL_LIB_OS_NAME		"FreeBSD"
#elif defined(__NetBSD__)||defined(__NetBSD)
	#define HGL_OS_NAME			u"NetBSD"
	#define HGL_LIB_OS_NAME		"NetBSD"
#elif defined(__OpenBSD__)||defined(__OPENBSD)
 	#define HGL_OS_NAME			u"OpenBSD"
 	#define HGL_LIB_OS_NAME		"OpenBSD"
#else
	#define HGL_OS_NAME			u"OtherBSD"
	#define HGL_LIB_OS_NAME		"OtherBSD"
#endif//

#define HGL_LIB_OS				"BSD"											//库操作系统前缀
//--------------------------------------------------------------------------------------------------
#define HGL_PLUGIN_FRONTNAME	"libCMP."										//插件文件名前缀
#define HGL_PLUGIN_EXTNAME		u".so"											//插件文件扩展名
#define HGL_PLUGIN_FUNC 		extern "C"										//插件函数定义

#define HGL_DIRECTORY_SEPARATOR u'/'											//目录分隔符

#define HGL_LINE_END			u"\n"											//换行符
#define HGL_LINE_END_SIZE		1												//换行符长度

#define HGL_MEM_ALIGN			16												//内存对齐字节数

#define HGL_MAX_PATH			_POSIX_PATH_MAX

#define HGL_FMT_I64				"%lld"
//--------------------------------------------------------------------------------------------------
#include<stdlib.h>
#include<pthread.h>

#define hgl_malloc(size)		malloc(size)
#define hgl_realloc(ptr,size)	realloc(ptr,size)
#define hgl_free				free

typedef pthread_mutex_t hgl_thread_mutex;

#define stat64					stat
#define open64					open
#define lseek64					lseek
#define tell64(fp)				lseek(fp,0,SEEK_CUR)
#define fstat64					fstat
#define lstat64					lstat
#define read64					read
#define write64					write
#define pread64					pread
#define pwrite64				pwrite

#define struct_stat64			struct stat
#define struct_dirent64 		struct dirent
#define dirent64				dirent
#define readdir64				readdir
//--------------------------------------------------------------------------------------------------
#endif//HGL_OS_BSD_INCLUDE
