#include<hgl/webapi/UserAgentString.h>

namespace hgl
{
    namespace webapi
    {
        UTF8String FirefoxUserAgent(FirefoxUserAgentConfig &cfg)
        {
            UTF8String agent="Mozilla/5.0 ";

            if(cfg.os>=OS_WindowsX86
             &&cfg.os<=OS_LinuxX86_64)
            {
                if(cfg.os>=OS_WindowsX86
                 &&cfg.os<=OS_WindowsWOW64)
                {
                    agent+="(Windows NT "+UTF8String(cfg.os_ver.major)+"."+UTF8String(cfg.os_ver.minor)+"; ";

                    if(cfg.os==OS_WindowsAMD64)agent+="Win64; x64; ";else
                    if(cfg.os==OS_WindowsWOW64)agent+="WOW64; ";
                }
                else
                if(cfg.os==OS_macOS)
                    agent+="(Macintosh; Intel Mac OS X "+UTF8String(cfg.os_ver.major)+"."+UTF8String(cfg.os_ver.minor)+"; ";
                else
                if(cfg.os==OS_Linuxi686)
                    agent+="(X11; Linux i686; ";
                else
                if(cfg.os==OS_LinuxX86_64)
                    agent+="(X11; Linux x86_64; ";

                agent+="rv:"+UTF8String(cfg.ff_ver.major)+"."+UTF8String(cfg.ff_ver.minor)+") Gecko/"+UTF8String(cfg.gecko_version)+" Firefox/"
                            +UTF8String(cfg.ff_ver.major)+"."+UTF8String(cfg.ff_ver.minor);
            }
            else
            if(cfg.os>=OS_iPod
             &&cfg.os<=OS_iPad)
            {
                if(cfg.os==OS_iPod) agent+="(iPod touch; ";else
                if(cfg.os==OS_iPad) agent+="(iPad; ";else
                                    agent+="(iPhone; ";

                agent+= "CPU iPhone OS "+UTF8String(cfg.os_ver.major)+"_"+UTF8String(cfg.os_ver.minor)+" like Mac OS X) "+
                        "AppleWebKit/600.1.4 (KHTML, like Gecko) FxiOS/1.0 Mobile/12F69 Safari/600.1.4";
            }
            else
            if(cfg.os>=OS_AndroidPhone
             &&cfg.os<=OS_AndroidTV)
            {
                agent+="(Android "+UTF8String(cfg.os_ver.major)+"."+UTF8String(cfg.os_ver.minor)+"; ";

                if(cfg.os==OS_AndroidPhone  )agent+="Mobile; ";
                if(cfg.os==OS_AndroidTablet )agent+="Tablet; ";
                if(cfg.os==OS_AndroidTV     )agent+="TV; ";

                agent+="rv:"+UTF8String(cfg.ff_ver.major)+"."+UTF8String(cfg.ff_ver.minor)+") Gecko/"
                            +UTF8String(cfg.ff_ver.major)+"."+UTF8String(cfg.ff_ver.minor)+" Firefox/"
                            +UTF8String(cfg.ff_ver.major)+"."+UTF8String(cfg.ff_ver.minor);
            }
            else
            {
                agent+="Firefox/"+UTF8String(cfg.ff_ver.major)+"."+UTF8String(cfg.ff_ver.minor);
            }

            return agent;
        }
    }//namespace webapi
}//namespace hgl
