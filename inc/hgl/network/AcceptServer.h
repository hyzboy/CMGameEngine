#ifndef HGL_NETWORK_ACCEPT_SERVER_INCLUDE
#define HGL_NETWORK_ACCEPT_SERVER_INCLUDE

#include<hgl/network/ServerSocket.h>
namespace hgl
{
	namespace network
	{
		/**
		 * 使用Accept处理接入的服务器基类
		 */
		class AcceptServer:public ServerSocket														///使用Accept创建接入的服务器基类
		{
		protected:

			virtual int CreateServerSocket()=0;														///<创建Server Socket

		protected:

			double overload_wait;

		public:

			AcceptServer()
			{
				overload_wait=HGL_SERVER_OVERLOAD_WAIT_TIME;
			}

			virtual ~AcceptServer()HGL_DEFAULT_MEMFUNC;

			virtual int Accept(sockaddr_in &);														///<接入一个socket连接
		};//class AcceptServer
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_ACCEPT_SERVER_INCLUDE
