#ifndef HGL_NETWORK_SOCKET_EVENT_INCLUDE
#define HGL_NETWORK_SOCKET_EVENT_INCLUDE
namespace hgl
{
	namespace network
	{
		/**
		 * Socket事件
		 */
		template<typename T> struct socket_event
		{
			T sock;			//socket号

			int size;		//数据长度,-1表示出错
		};

		typedef socket_event<int> SocketEvent;

		class Socket;
		class IOSocket;

		typedef socket_event<Socket *> SocketClassEvent;
		typedef socket_event<IOSocket *> IOSocketClassEvent;
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SOCKET_EVENT_INCLUDE
