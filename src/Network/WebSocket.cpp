#include<hgl/Str.h>
#include<hgl/type/BaseString.h>
#include<hgl/io/MemoryOutputStream.h>
#include<hgl/algorithm/Hash.h>
#include<hgl/algorithm/Crypt.h>

namespace hgl
{
    namespace network
    {
        /**
         * 获取WebSocket信息
         * @param data 输入的信息头
         * @param size 信息头长度
         * @return 是否解晰成功
         */
        bool GetWebSocketInfo(UTF8String &sec_websocket_key,UTF8String &sec_websocket_protocol,uint &sec_websocket_version,const char *data,int size)
        {
            constexpr char SEC_WEBSOCKET_KEY_STR[]="Sec-WebSocket-Key: ";
            constexpr uint SEC_WEBSOCKET_KEY_STR_SIZE=sizeof(SEC_WEBSOCKET_KEY_STR);

            constexpr char SEC_WEBSOCKET_PROTOCOL[]="Sec-WebSocket-Protocol: ";
            constexpr uint SEC_WEBSOCKET_PROTOCOL_SIZE=sizeof(SEC_WEBSOCKET_PROTOCOL);

            constexpr char SEC_WEBSOCKET_VERSION[]="Sec-WebSocket-Version: ";
            constexpr uint SEC_WEBSOCKET_VERSION_SIZE=sizeof(SEC_WEBSOCKET_VERSION);

            if(!data||size<40)return(false);
            
            const char *end;

            {
                const char *key=hgl::strstr(data,size,SEC_WEBSOCKET_KEY_STR);

                if(!key)return(false);

                key+=SEC_WEBSOCKET_KEY_STR_SIZE;

                end=key;
                while(*end!='\n')++end;     //找到非BASE64字符为止

                sec_websocket_key.Set(key,end-key);
            }

            {
                const char *protocol=hgl::strstr(end,size,SEC_WEBSOCKET_PROTOCOL);

                if(!protocol)return(false);

                protocol+=SEC_WEBSOCKET_PROTOCOL_SIZE;
                end=protocol;
                while(*end!='\n')++end;

                sec_websocket_protocol.Set(protocol,end-protocol);
            }

            {
                const char *version=hgl::strstr(end,size,SEC_WEBSOCKET_VERSION);

                if(!version)return(false);

                version+=SEC_WEBSOCKET_VERSION_SIZE;
                end=version;
                while(*end!='\n')++end;

                return hgl::stou(version,sec_websocket_version);
            }
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
                   "Sec-WebSocket-Accept: "+sec_websocket_accept+"\r\n"
                   "Sec-WebSocket-Protocol: "+sec_websocket_protocol+"\r\n\r\n";
        }
    }//namespace network
}//namespace hgl
