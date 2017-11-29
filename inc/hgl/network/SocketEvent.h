#ifndef HGL_NETWORK_SOCKET_EVENT_INCLUDE
#define HGL_NETWORK_SOCKET_EVENT_INCLUDE

#include<hgl/platform/Platform.h>
#include<hgl/type/List.h>
namespace hgl
{
    namespace network
    {
        struct SocketEvent
        {
            int sock;

            union
            {
                int size;           //数据长度(此属性为BSD系统独有)
                int error;          //错误号
            };
        };//struct SocketEvent

        using SocketEventList=List<SocketEvent>;
    }//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SOCKET_EVENT_INCLUDE
