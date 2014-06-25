#ifndef HGL_NETWORK_SCTP_CLIENT_INCLUDE
#define HGL_NETWORK_SCTP_CLIENT_INCLUDE

#include<hgl/network/SCTPSocket.h>
namespace hgl
{
	namespace network
	{
		/**
		 * SCTP一对一模式(one to one,tcp-style)客户端<br>
		 * SCTP一对一模式(one to one,tcp-style)通信处理的客户端封装
		 */
		class SCTPO2OClient:public SCTPO2OSocket														///SCTP Client (tcp-style)
		{
		public:

			SCTPO2OClient();
			SCTPO2OClient(int);
			~SCTPO2OClient();

			virtual bool Connect(const sockaddr_in &);													///<连接到服务器
					bool Connect(const char *,int);														///<接连到服务器
		};//class SCTPO2OClient:public Socket
	}//namespace network

	using namespace network;
}//namespace hgl
#endif//HGL_NETWORK_SCTP_CLIENT_INCLUDE
