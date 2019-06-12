#ifndef HGL_EXTERNAL_CORE_INCLUDE
#define HGL_EXTERNAL_CORE_INCLUDE

#include<hgl/platform/platform.h>
#include<hgl/platform/ConsoleSystemInfo.h>
#include<hgl.h>

namespace hgl
{
    bool InitCore(ConsoleSystemInfo &si,bool create_log);   ///<初始化核心
    void CloseCore();                                       ///<关闭核心
}//namespace hgl
#endif//HGL_EXTERNAL_CORE_INCLUDE
