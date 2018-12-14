#include<hgl/Console.h>
#include<hgl/webapi/UserAgentString.h>

#include<iostream>

using namespace hgl;
using namespace hgl::webapi;

int main(int,char **)
{
    FirefoxUserAgentConfig cfg;

    cfg.os              =OS_iPhone;
    cfg.ff_ver.major    =63;
    cfg.ff_ver.minor    =0;
    cfg.gecko_version   =20181212;

    cfg.os_ver.major    =12;
    cfg.os_ver.minor    =1;


    UTF8String user_agent=FirefoxUserAgent(cfg);

    std::cout<<"iPhone: "<<user_agent.c_str()<<std::endl;

    cfg.os=OS_iPad;
    user_agent=FirefoxUserAgent(cfg);
    std::cout<<"iPad: "<<user_agent.c_str()<<std::endl;

    cfg.os=OS_AndroidPhone;
    cfg.os_ver.major=9;
    user_agent=FirefoxUserAgent(cfg);
    std::cout<<"Android 9: "<<user_agent.c_str()<<std::endl;

    cfg.os=OS_WindowsAMD64;
    cfg.os_ver.major=10;
    user_agent=FirefoxUserAgent(cfg);
    std::cout<<"Windows 10: "<<user_agent.c_str()<<std::endl;

    return 0;
}

