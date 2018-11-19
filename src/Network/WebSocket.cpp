#include<hgl/Str.h>
#include<hgl/type/BaseString.h>
#include<hgl/io/MemoryOutputStream.h>
#include<hgl/algorithm/Hash.h>
#include<hgl/algorithm/Crypt.h>

namespace hgl
{
    namespace network
    {
        using namespace hgl::algorithm;

        /**
         * 获取WebSocket信息
         * @param data 输入的信息头
         * @param size 信息头长度
         * @return 是否解晰成功
         */
        bool GetWebSocketInfo(UTF8String &sec_websocket_key,UTF8String &sec_websocket_protocol,uint &sec_websocket_version,const char *data,const uint size)
        {
            constexpr char SEC_WEBSOCKET_KEY[]="Sec-WebSocket-Key: ";
            constexpr uint SEC_WEBSOCKET_KEY_SIZE=sizeof(SEC_WEBSOCKET_KEY)-1;      //sizeof的带\0所以要-1

            constexpr char SEC_WEBSOCKET_PROTOCOL[]="Sec-WebSocket-Protocol: ";
            constexpr uint SEC_WEBSOCKET_PROTOCOL_SIZE=sizeof(SEC_WEBSOCKET_PROTOCOL)-1;

            constexpr char SEC_WEBSOCKET_VERSION[]="Sec-WebSocket-Version: ";
            constexpr uint SEC_WEBSOCKET_VERSION_SIZE=sizeof(SEC_WEBSOCKET_VERSION)-1;

            if(!data||size<40)return(false);

            const char *end;

            {
                const char *key=hgl::strstr(data,size,SEC_WEBSOCKET_KEY,SEC_WEBSOCKET_KEY_SIZE);

                if(!key)return(false);

                key+=SEC_WEBSOCKET_KEY_SIZE;

                end=key;
                while(*end!='\r')++end;

                sec_websocket_key.Set(key,end-key);
            }

            {
                const char *protocol=hgl::strstr(data,size,SEC_WEBSOCKET_PROTOCOL,SEC_WEBSOCKET_PROTOCOL_SIZE);

                if(protocol)        //也有可能是不存在的
                {
                    protocol+=SEC_WEBSOCKET_PROTOCOL_SIZE;
                    end=protocol;
                    while(*end!='\r')++end;

                    sec_websocket_protocol.Set(protocol,end-protocol);
                }
            }

            {
                const char *version=hgl::strstr(data,size,SEC_WEBSOCKET_VERSION,SEC_WEBSOCKET_VERSION_SIZE);

                if(version)
                {
                    version+=SEC_WEBSOCKET_VERSION_SIZE;
                    end=version;
                    while(*end!='\r')++end;

                    hgl::stou(version,sec_websocket_version);
                }
            }

            return(true);
        }

        /**
         * 生成WebSocket回复头
         * @param result 回复头存放字符串
         */
        void MakeWebSocketAccept(UTF8String &result,const UTF8String &sec_websocket_key,const UTF8String &sec_websocket_protocol)
        {
            const UTF8String key_mask=sec_websocket_key+"258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

            HashCodeSHA1 hc;

            CountSHA1(key_mask.c_str(),key_mask.Length(),hc);

            io::MemoryOutputStream mos;

            base64_encode(&mos,hc.code,hc.size());

            const UTF8String sec_websocket_accept((char *)mos.GetData(),mos.GetSize());

            result="HTTP/1.1 101 Switching Protocols\r\n"
                   "Upgrade: websocket\r\n"
                   "Connection: Upgrade\r\n"
                   "Sec-WebSocket-Accept: "+sec_websocket_accept;

            if(!sec_websocket_protocol.IsEmpty())
                result+="\r\nSec-WebSocket-Protocol: "+sec_websocket_protocol;

            result+="\r\n\r\n";
        }
    }//namespace network
}//namespace hgl
