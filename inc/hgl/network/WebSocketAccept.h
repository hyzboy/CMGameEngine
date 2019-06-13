#ifndef HGL_NETWORK_WEBSOCKET_ACCEPT_INCLUDE
#define HGL_NETWORK_WEBSOCKET_ACCEPT_INCLUDE

#include<hgl/network/TCPAccept.h>
#include<hgl/type/BaseString.h>
namespace hgl
{
    namespace network
    {
        /**
         * WebSocket接入管理
         */
        class WebSocketAccept:public TCPAccept
        {
        #ifdef _DEBUG
            MemBlock<char> data_out_str;
        #endif//_DEBUG
        protected:

            MemBlock<uchar> recv_buffer;
            uint            recv_length=0;

            uint64          recv_total=0;

        protected:

            virtual int OnSocketRecv(int) override;                                      ///<Socket接收处理函数

            void WebSocketHandshake();
            int SendFrame(uint8,void *,uint32,bool);

        protected:

            uint    msg_opcode;
            bool    msg_fin;
            bool    msg_masked;
            uint    msg_header_size;
            uint64  msg_length;
            uint64  msg_full_length;

            uint    last_opcode;

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

            WebSocketAccept();                                                  ///<本类构造函数
            WebSocketAccept(int,IPAddress *);                                   ///<本类构造函数
            virtual ~WebSocketAccept()=default;

            virtual void OnPing(){}
            virtual void OnPong(){}
            virtual bool OnBinary(void *,uint32,bool)=0;
            virtual bool OnText(char *,uint32,bool)=0;
            virtual void OnError(){}

            bool SendPing();
            bool SendPong();
            bool SendBinary(void *,uint32,bool=true);
            bool SendText(void *,uint32,bool=true);

            bool SendText(const UTF8String &str)
            {
                return SendText(str.c_str(),str.Length());
            }
        };//class WebSocketAccept:public TCPAccept
    }//namespace network
}//namespace hgl
#endif//HGL_NETWORK_WEBSOCKET_ACCEPT_INCLUDE
