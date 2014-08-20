#include<hgl/network/ServerSocket.h>
#include<hgl/LogInfo.h>
#include<hgl/Other.h>

namespace hgl
{
	namespace network
	{
		ServerSocket::ServerSocket()
		{
			ThisSocket=-1;
		}

		ServerSocket::~ServerSocket()
		{
			CloseServer();
		}

		/**
		* 创建服务器
		* @param addr 服务器地址
		* @param max_listen 最大监听数量(指同一时间在未处理的情况下，最多有多少个连接可以被处理。注：并非越大越好)
		* @return 创建服务器是否成功
		*/
		bool ServerSocket::CreateServer(const sockaddr_in &addr,const uint max_listen)
		{
			ThisSocket=CreateServerSocket();

			if(ThisSocket<0)
			{
				LOG_HINT(OS_TEXT("Create TCP Server Socket Failed!"));
				return(false);
			}

			if(!BindAddr(ThisSocket,addr))
			{
				CloseSocket(ThisSocket);
        		return(false);
			}

			listen(ThisSocket,max_listen);

			return(true);
		}

		/**
		* 创建服务器
		* @param hostname 指要监听的服务器地址
		* @param port 指要监听的服务器端口
		* @param max_listen 最大监听数量(指同一时间在未处理的情况下，最多有多少个连接可以被处理。注：并非越大越好)
		* @return 创建服务器是否成功
		*/
		bool ServerSocket::CreateServer(const char *hostname,uint port,const uint max_listen)
		{
			sockaddr_in addr;

			if(!FillAddr(&addr,hostname,port))
				return(false);

			if(!CreateServer(addr,max_listen))
			{
				LOG_HINT(OS_TEXT("ServerSocket::CreateServer failed,Listen Port: ")+OSString(port));

				return(false);
			}

			LOG_HINT(U8_TEXT("ServerSocket Listen Address: ")+UTF8String(hostname));
			LOG_HINT(OS_TEXT("ServerSocket Listen Port: ")+OSString(port));

			return(true);
		}

		void ServerSocket::CloseServer()
		{
			CloseSocket(ThisSocket);
			ThisSocket=-1;
		}
	}//namespace network
}//namespace hgl
