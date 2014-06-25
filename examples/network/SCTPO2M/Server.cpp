#include<hgl/network/SCTPServer.h>
#include<hgl/network/SCTPSocket.h>
#include<hgl/thread/Thread.h>

using namespace hgl;
using namespace hgl::network;

int main(int,char **)
{
	SCTPO2MServer server;

	if(!server.CreateServer("0.0.0.0",10000))							//创建一个Server监听
	{
		std::cout<<"create server failed!"<<std::endl;
		return(-1);
	}

	sockaddr_in sa;
	char buf[1024];
	int len;
	uint16 stream;

	while(true)
	{
		if(server.RecvMsg(sa,buf,1024,len,stream))
		{
			strcat(buf," by server");
			if(!server.SendMsg(&sa,buf,strlen(buf)+1,stream))
			{
				std::cout<<"send msg error!"<<std::endl;
			}
		}
		else
		{
			std::cout<<"recv msg error!"<<std::endl;
		}
	}

	server.CloseServer();
	return(0);
}
