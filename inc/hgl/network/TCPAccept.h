#ifndef HGL_NETWORK_TCP_ACCEPT_INCLUDE
#define HGL_NETWORK_TCP_ACCEPT_INCLUDE

#include<hgl/network/TCPSocket.h>
#include<hgl/thread/RingBuffer.h>
namespace hgl
{
    namespace network
    {
        struct SocketEvent;

        /**
         * TCP服务器接入用户处理基类，为各种Server管理器提供统一调用接口
         */
        class TCPAccept:public TCPSocket
        {
        protected:  //事件函数，由SocketManage调用

            friend class SocketManage;

            virtual int OnSocketRecv(int)=0;                                    ///<Socket接收处理函数
            virtual int OnSocketSend(int)=0;                                    ///<Socket发送处理函数
            virtual void OnSocketError(int){}                                   ///<Socket错误处理函数

        public:

            using TCPSocket::TCPSocket;

            virtual ~TCPAccept()=default;
        };//class TCPAccept:public TCPSocket

        /**
         * 带环形数据缓冲区的TCPAccept
         */
        class RBTCPAccept:public TCPAccept
        {
        protected:

            RingBuffer<char> *buffer;                                           ///<TCP ACCEPT专用环形缓冲区

        protected:

            virtual int OnSocketRecv(int);                                      ///<Socket接收事件
            virtual int OnSocketSend(int);                                      ///<Socket发送事件

        public:

            using TCPAccept::TCPAccept;

            virtual ~RBTCPAccept()=default;

        };//class RBTCPAccept:public TCPAccept
    }//namespace network
}//namespace hgl
#endif//HGL_NETWORK_TCP_ACCEPT_INCLUDE
