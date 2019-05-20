#ifndef HGL_TCP_SERVER_INCLUDE
#define HGL_TCP_SERVER_INCLUDE

#include<hgl/network/AcceptServer.h>
namespace hgl
{
	namespace network
	{
		/**
		* TCPServer是对应TCP连接处理的通用服务器端
		*/
		class TCPServer:public AcceptServer															///TCP服务器端实现基类
		{
			int	CreateServerSocket() override final;											///<创建一个服务器用Socket

		public:

#if (HGL_OS != HGL_OS_Windows)&&(HGL_OS != HGL_OS_macOS)
			void SetDeferAccept(const int);															///<设置推迟Accept
#endif//no windows&mac
		};//class TCPServer
	}//namespace network

	using namespace network;
}//namespace hgl
#endif//HGL_TCP_SERVER_INCLUDE
