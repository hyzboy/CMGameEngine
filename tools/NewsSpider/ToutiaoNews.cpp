#include<hgl/Console.h>
#include<hgl/network/HTTPTools.h>
#include<hgl/io/MemoryOutputStream.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/TextOutputStream.h>
#include<hgl/LogInfo.h>
#include<hgl/type/StringList.h>
#include<hgl/webapi/UserAgentString.h>

#include<gumbo-parser/gumbo.h>      //google html5 解晰库

#include<iostream>

using namespace hgl;
using namespace hgl::io;
using namespace hgl::filesystem;
using namespace hgl::webapi;

HGL_CONSOLE_MAIN_FUNC()
{
    FirefoxUserAgentConfig cfg;

    cfg.os              =OS_iPhone;

    cfg.os_ver.major    =12;
    cfg.os_ver.minor    =1;

    cfg.ff_ver.major    =63;
    cfg.ff_ver.minor    =0;

    UTF8String user_agent=FirefoxUserAgent(cfg);

    std::cout<<"iPhone: "<<user_agent.c_str()<<std::endl;

    cfg.os=OS_iPad;
    user_agent=FirefoxUserAgent(cfg);
    std::cout<<"iPad: "<<user_agent.c_str()<<std::endl;

    cfg.os=OS_AndroidPhone;
    cfg.os_ver.major=9;
    user_agent=FirefoxUserAgent(cfg);
    std::cout<<"Android 9: "<<user_agent.c_str()<<std::endl;

    return 0;
}
