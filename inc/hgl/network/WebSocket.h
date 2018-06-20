#ifndef HGL_NETWORK_WEBSOCKET_INCLUDE
#define HGL_NETWORK_WEBSOCKET_INCLUDE

#include<hgl/type/BaseString.h>
namespace hgl
{
    namespace network
    {
        bool GetWebSocketInfo(UTF8String &sec_websocket_key,UTF8String &sec_websocket_protocol,uint &sec_websocket_version,const char *data,const uint size);
        void MakeWebSocketAccept(UTF8String &result,const UTF8String &sec_websocket_key,const UTF8String &sec_websocket_protocol);
    }//namespace network
}//namespace hgl
#endif//HGL_NETWORK_WEBSOCKET_INCLUDE
