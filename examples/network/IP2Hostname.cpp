#include<hgl/Console.h>        //HGL_CONSOLE_APPLICATION宏定义
#include<hgl/network/IP.h>
#include<iostream>

using namespace hgl;
using namespace hgl::network;

HGL_CONSOLE_MAIN(sii,app,args)
{
    std::cout<<"IP to Hostname tools (Powered by CMGDK)"<<std::endl;
    std::cout<<std::endl;

    if(args.GetCount()!=3)
    {
        std::cout<<"format: IP2Hostname [4|6] ip"<<std::endl;
        std::cout<<std::endl;
        std::cout<<"example: IP2Hostname 4 127.0.0.1"<<std::endl;

        return 0;
    }

    IPAddress *ip;
    UTF8String hostname;

    if(args[1]=='4')ip=new network::IPv4Address;else
    if(args[1]=='6')ip=new network::IPv6Address;else
        return(-1);

    ip->Set(args[2],0,0,0);

    if(ip->GetHostname(hostname))
        std::cout<<"hostname: "<<hostname.c_str()<<std::endl;
    else
        std::cout<<"get hostname error"<<std::endl;

    return 0;
}
