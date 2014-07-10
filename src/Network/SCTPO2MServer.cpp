#include<hgl/network/SCTPServer.h>
#include<hgl/LogInfo.h>
#include<netinet/sctp.h>

namespace hgl
{
	namespace network
	{
		/**
		* 创建服务器
		* @param addr 服务器地址
		* @param max_listen 最大监听数量(指同一时间在未处理的情况下，最多有多少个连接可以被处理。注：并非越大越好)
		* @return 创建服务器是否成功
		*/
		bool SCTPO2MServer::CreateServer(const sockaddr_in &addr,const uint max_listen)
		{
			if(!SCTPO2MSocket::Create())
				return(false);

			if(!BindAddr(ThisSocket,addr))
			{
				CloseSocket();
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
		bool SCTPO2MServer::CreateServer(const char *hostname,uint port,const uint max_listen)
		{
			sockaddr_in addr;

			if(!FillAddr(&addr,hostname,port))
				return(false);

			if(!CreateServer(addr,max_listen))
			{
				LOG_HINT(OS_TEXT("ServerSocket::CreateServer failed,Listen Port: ")+OSString(port));

				return(false);
			}

			LOG_HINT(u8"ServerSocket Listen Address: "+UTF8String(hostname));
			LOG_HINT(OS_TEXT("ServerSocket Listen Port: ")+OSString(port));

			return(true);
		}

		void SCTPO2MServer::CloseServer()
		{
			CloseSocket();
		}

		/**
		 * 设置自动关闭无通信的客户端
		 * @param to 超时时间
		 */
		void SCTPO2MServer::SetAutoClose(const int to)
		{
			setsockopt(ThisSocket,IPPROTO_SCTP,SCTP_AUTOCLOSE,&to,sizeof(int));
		}
	}//namespace network
}//namespace hgl
