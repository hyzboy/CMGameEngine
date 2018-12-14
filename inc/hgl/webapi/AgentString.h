#ifndef HGL_WEBAPI_BROWSER_AGENT_STRING_INCLUDE
#define HGL_WEBAPI_BROWSER_AGENT_STRING_INCLUDE

#include<hgl/type/BaseString.h>
namespace hgl
{
    namespace webapi
    {
        enum OS
        {
            OS_WindowsX86=0,
            OS_WindowsAMD64,
            OS_WindowsWOW64,

            OS_macOS,

            OS_Linuxi686,
            OS_LinuxX86_64,

            OS_iPod,
            OS_iPhone,
            OS_iPad,

            OS_AndroidPhone,
            OS_AndroidTablet,
            OS_AndroidTV,

            OS_END
        };

        struct FirefoxUserAgentConfig
        {
            hgl::webapi::OS os;

            uint64 gecko_version;

            struct
            {
                int major;
                int minor;
            }
            ff_ver,                 //firefox版本号
            os_ver;                 //操作系统版本号
        };

        /**
         * 生成一个firefox用户代理字符串
         */
        UTF8String FirefoxUserAgent(FirefoxUserAgentConfig &cfg);
    }//namespace webapi
}//namespace hgl
#endif//HGL_WEBAPI_BROWSER_AGENT_STRING_INCLUDE
