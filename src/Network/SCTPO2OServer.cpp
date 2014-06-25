#include<hgl/network/SCTPServer.h>

namespace hgl
{
	namespace network
	{
		int SCTPO2OServer::CreateServerSocket()
		{
			return socket(AF_INET,SOCK_STREAM,IPPROTO_SCTP);
		}
	}//namespace network
}//namespace hgl
