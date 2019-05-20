#include<hgl/Console.h>
#include<hgl/Time.h>
#include<hgl/network/MTTCPServer.h>
#include<iostream>

using namespace hgl;
using namespace hgl::network;

class UserAccept:public TCPAcceptPacket
{
public:

    using TCPAcceptPacket::TCPAcceptPacket;
    ~UserAccept()=default;

    void OnSocketError(int err_no) override
    {
        std::cout<<"socket error,errno:"<<err_no<<std::endl;
    }

    bool OnRecvPacket(void *data,const PACKET_SIZE_TYPE &size) override
    {
        std::string str((char *)data,size);

        std::cout<<"recv packet "<<str.c_str()<<std::endl;

        char *s=(char *)data;
        char *e=s+size-1;
        char t;

        while(s<e)
        {
            t=*s;
            *s=*e;
            *e=t;

            ++s;
            --e;
        }

        return SendPacket(data,size);       //再发回去
    }
};//class UserAccept:public TCPAccept

using GameServer=MTTCPServerStd<UserAccept>;

HGL_CONSOLE_MAIN(sii,app,args)
{
    sii.info.ProjectName=U8_TEXT("多线程Echo TCP服务器");
    sii.info.ProjectCode=OS_TEXT("MTEchoTCPServer");

    if(!app.Init(&sii))
        return(-1);

#if HGL_OS == HGL_OS_Windows
    InitWinSocket();
#endif//

    IPAddress *server_ip;

    server_ip=CreateIPv4TCP(10240);

    if(!server_ip)
        return(-2);

    GameServer server;
    GameServer::InitInfomation info;

    info.server_ip=server_ip;
    info.thread_count=1;

    if(!server.Init(info))
        return(-3);

    while(server.Wait());      //等待服务器关闭
    return(0);
}

