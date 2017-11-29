#ifndef HGL_NETWORK_SOCKET_EVENT_INCLUDE
#define HGL_NETWORK_SOCKET_EVENT_INCLUDE

#include<hgl/type/List.h>
namespace hgl
{
	namespace network
	{
		/**
		 * Socket事件
		 */
		struct SocketEvent
		{
            int sock;

            uint event;

            union
            {
                int size;
                int error;
            };
        };//struct SocketEvent

        using SocketEventList=List<SocketEvent>;
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SOCKET_EVENT_INCLUDE
