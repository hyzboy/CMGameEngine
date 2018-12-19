#include<hgl/Console.h>
#include<hgl/webapi/UserAgentString.h>

using namespace hgl;
using namespace hgl::webapi;

const UTF8String &InitUserAgent()
{
    FirefoxUserAgentConfig cfg;

    cfg.os              =OS_WindowsAMD64;

    cfg.os_ver.major    =10;
    cfg.os_ver.minor    =0;

    cfg.ff_ver.major    =64;
    cfg.ff_ver.minor    =0;

    return FirefoxUserAgent(cfg);
}

void news_hrb(const UTF8String &);

HGL_CONSOLE_MAIN_FUNC()
{
    const UTF8String &user_agent=InitUserAgent();

    news_hrb(user_agent);     //哈尔滨

    return 0;
}

