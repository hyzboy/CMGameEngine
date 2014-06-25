#ifndef HGL_OS_MACOS_INCLUDE
#define HGL_OS_MACOS_INCLUDE

#define HGL_OS_NAME				u"MacOS"

#define HGL_PLUGIN_FRONTNAME	"libCMP."										//插件文件名前缀
#define HGL_PLUGIN_EXTNAME		u".dylib"										//插件文件扩展名
#define HGL_PLUGIN_FUNC 		extern "C"										//插件函数定义

#define HGL_DIRECTORY_SEPARATOR L'/'											//目录分隔符
#define HGL_LINE_END			u"\r"											//换行符
#define HGL_LINE_END_SIZE		1												//换行符长度

#define HGL_MAX_PATH			256
#define HGL_MEM_ALIGN			16												//内存对齐字节数

#include<pthread.h>

typedef pthread_mutex_t hgl_thread_mutex;

#define iconv_str				const char *

#endif//HGL_OS_MACOS_INCLUDE
