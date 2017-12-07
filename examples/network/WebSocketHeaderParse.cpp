#include<hgl/network/WebSocket.h>
#include<hgl/Str.h>
#include<iostream>

using namespace hgl;
using namespace hgl::network;

using namespace std;

int main(int,char **)
{
    constexpr char http_header[]=   "GET /chat HTTP/1.1\r\n"
                                    "Host: server.example.com\r\n"
                                    "Upgrade: websocket\r\n"
                                    "Connection: Upgrade\r\n"
                                    "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
                                    "Origin: http://example.com\r\n"
                                    "Sec-WebSocket-Protocol: chat, superchat\r\n"
                                    "Sec-WebSocket-Version: 13\r\n\r\n";
    const uint http_header_length=hgl::strlen(http_header);
    constexpr char split_line[]="---------------------------------------------------\n";

    UTF8String key;
    UTF8String protocol;
    uint version=0;
    UTF8String reply;

    cout<<"Origin HTTP Header:"<<endl<<split_line<<http_header<<split_line;

    GetWebSocketInfo(key,protocol,version,http_header,http_header_length);

    cout<<"WebSocket Key:      "<<key.c_str()<<endl;

    if(!protocol.IsEmpty())
    cout<<"WebSocket Protocol: "<<protocol.c_str()<<endl;

    cout<<"WebSocket Version:  "<<version<<endl<<split_line;

    MakeWebSocketAccept(reply,key,protocol);

    cout<<"WebSocket Reply:    "<<endl<<split_line<<reply.c_str()<<endl;

    return 0;
}
