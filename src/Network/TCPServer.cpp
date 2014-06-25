#include<hgl/network/TCPServer.h>

namespace hgl
{
	namespace network
	{
		int TCPServer::CreateServerSocket()
		{
			//独立一个函数，是因为Windows下创建IOCP Server时，这里需要特殊实现
			return socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		}
	}//namespace network
}//namespace hgl
