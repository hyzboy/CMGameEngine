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
                #if HGL_OS==HGL_OS_FreeBSD||HGL_OS==HGL_OS_OpenBSD||HGL_OS==HGL_OS_NetBSD||HGL_OS==HGL_OS_macOS||HGL_OS==HGL_OS_iOS
                int size;           //数据长度(此属性为BSD系统独有)
                #endif

                int error;          //错误号
            };
        };//struct SocketEvent

        using SocketEventList=List<SocketEvent>;
    }//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SOCKET_EVENT_INCLUDE
