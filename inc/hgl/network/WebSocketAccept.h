#ifndef HGL_NETWORK_WEBSOCKET_ACCEPT_INCLUDE
#define HGL_NETWORK_WEBSOCKET_ACCEPT_INCLUDE

#include<hgl/network/TCPAccept.h>
namespace hgl
{
    namespace network
    {
        /**
         * WebSocket接入管理 
         */
        class WebSocketAccept:public TCPAccept
        {
        protected:

            virtual int OnSocketRecv(int) override;                                      ///<Socket接收处理函数

            void WebSocketHandshake();

        protected:

            uint    msg_opcode;
            bool    msg_fin;
            bool    msg_masked;
            uint    msg_header_size;
            uint64  msg_length;
            uint64  msg_full_length;

        protected: //WebSocket支持

            /**
             * WebSocket 握手响应
             * @param accept_protocol 接受的WebSocket协议
             * @param protocol 需求的WebSocket协议
             * @param version 需求的WebSocket版本
             * @return 是否成功
             */
            virtual bool OnHandshake(UTF8String &accept_protocol,const UTF8String &protocol,uint version)
            {
                accept_protocol=protocol;           //原样接收，如有需求请自行重载此函数处理
                return(true);
            }

        public:

            using TCPAccept::TCPAccept;
            virtual ~WebSocketAccept()=default;

            virtual void OnPing(){}
            virtual void OnPong(){}
            virtual bool OnBinary(void *,uint32,bool);
            virtual bool OnText(char *,uint32,bool);
            virtual void OnError(){}
        };//class WebSocketAccept:public TCPAccept
    }//namespace network
}//namespace hgl
#endif//HGL_NETWORK_WEBSOCKET_ACCEPT_INCLUDE
