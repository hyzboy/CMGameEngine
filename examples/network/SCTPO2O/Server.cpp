#include<hgl/network/SCTPServer.h>
#include<hgl/network/SCTPSocket.h>
#include<hgl/thread/Thread.h>

using namespace hgl;
using namespace hgl::network;

class SCTPEchoServer:public Thread
{
private:

	SCTPO2OSocket sock;

	MemBlock<char> mb;
	uint16 stream;

public:

    SCTPEchoServer(int s)
	{
		sock.UseSocket(s);

		std::cout<<"SCTPEchoServer["<<sock.ThisSocket<<"] Start"<<std::endl;
	}

    ~SCTPEchoServer()
	{
		std::cout<<"SCTPEchoServer["<<sock.ThisSocket<<"] End"<<std::endl;
	}

	bool Execute()
	{
		bool result=sock.RecvMsg(&mb,stream);

		if(result)
		{
			std::cout<<"recv msg ["<<sock.ThisSocket<<"] stream:"<<stream<<"\t"<<mb.GetData()<<std::endl;

			result=sock.SendMsg(mb.GetData(),mb.GetLength(),stream);
		}
		else
		{
			int err=GetLastSocketError();

			if(err==nseTryAgain)
				return(true);

			std::cout<<"recv msg ["<<sock.ThisSocket<<"] errno:"<<err<<std::endl;
		}

		return result;
	}
};//class SCTPEchoServer

int main(int,char **)
{
	SCTPO2OServer server;

	if(!server.CreateServer("0.0.0.0",10000))							//创建一个Server监听
	{
		std::cout<<"create server failed!"<<std::endl;
		return(-1);
	}

	sockaddr_in sa;

	while(true)
	{
		int sock=server.Accept(sa);										//获取一个新的socket接入

		if(sock<=0)
			continue;

		SCTPEchoServer *ses=new SCTPEchoServer(sock);

		ses->Thread::Start();											//启动线程
	}

	server.CloseServer();
	return(0);
}
