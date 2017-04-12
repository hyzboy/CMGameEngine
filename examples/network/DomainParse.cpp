#include<hgl/Console.h>        //HGL_CONSOLE_APPLICATION宏定义
#include<hgl/network/Socket.h>
#include<hgl/network/IP.h>
#include<hgl/type/Map.h>
#include<iostream>

using namespace hgl;

Map<UTF8String,Pair<int,int>> it_list;

void InitITList()
{
    it_list.Add("TCP",  Pair<int,int>(SOCK_STREAM,   IPPROTO_TCP ));
    it_list.Add("UDP",  Pair<int,int>(SOCK_DGRAM,    IPPROTO_UDP )),
    it_list.Add("SCTP", Pair<int,int>(SOCK_SEQPACKET,IPPROTO_SCTP));
}

template<typename T_IPADDRESS,typename T_IN_ADDR,int ADDRSTRLEN>
void parse(const UTF8String &type_string,const UTF8String &domain)
{
    T_IPADDRESS ipa;

    List<T_IN_ADDR> ip_list;
    Pair<int,int> tp;

    char addr_string[ADDRSTRLEN+1];

    InitITList();                   //初始化协议映射表

    if(!it_list.Get(type_string,tp))
    {
        std::cout<<"ipproto parse error"<<std::endl;

        return;
    }

    ipa.GetDomainIPList(ip_list,domain,tp.left,tp.right);

    for(int i=0;i<ip_list.GetCount();i++)
    {
        ipa.ToString(addr_string,ip_list[i]);

        std::cout<<addr_string<<::std::endl;
    }
}

HGL_CONSOLE_MAIN(sii,app,args)
{
    std::cout<<"Domain parse tools (Powered by CMGDK)"<<std::endl;
    std::cout<<std::endl;

    if(args.GetCount()!=4)
    {
        std::cout<<"format: DomainParse [4|6] [TCP|UDP|SCTP] domain"<<std::endl;
        std::cout<<std::endl;
        std::cout<<"example: DomainParse 4 TCP google.com"<<std::endl;

        return 0;
    }

    #if HGL_OS == HGL_OS_Windows
        UTF8String type=to_u8(args[2]);
        UTF8String domain=to_u8(args[3]);

        if(!network::InitWinSocket())
        {
            std::cout<<"Init WinSocket error!"<<std::endl;
            return(1);
        }

        if(args[1]==L'4')parse<network::IPv4Address,in_addr,INET_ADDRSTRLEN>(type,domain);else
        if(args[1]==L'6')parse<network::IPv6Address,in6_addr,INET6_ADDRSTRLEN>(type,domain);
    #else
        if(args[1]=='4')parse<network::IPv4Address,in_addr,INET_ADDRSTRLEN>(args[2],args[3]);else
        if(args[1]=='6')parse<network::IPv6Address,in6_addr,INET6_ADDRSTRLEN>(args[2],args[3]);
    #endif//HGL_OS == HGL_OS_Windows

    return 0;
}
