#ifndef HGL_NETWORK_TCP_ACCEPT_INCLUDE
#define HGL_NETWORK_TCP_ACCEPT_INCLUDE

#include<hgl/network/TCPSocket.h>
#include<hgl/type/MemBlock.h>
namespace hgl
{
    namespace network
    {
        /**
         * TCPAccept与SocketManage
         *
         * TCPAccept:           客户端连接到服务器后的控制对象
         * SocketManage:        服务器对批量TCPAccept进行管理的对象
         * SocketManageThread:  SocketManage的异步封装
         *
         * 收包流程:
         *          1.SocketManage通过OnSocketRecv函数通知TCPAccept接收数据
         *          2.TCPAccept在收满4个字节的包长后，根据包长向MultiLevelMemoryPool申请缓冲区
         *          3.TCPAccept收满一个包后，通过OnRecvPacket事件函数通知开发者有包了
         *          4.TCPAccept将缓冲区送回BufferPool
         */


        using PACKET_SIZE_TYPE=uint32;                                          ///<描述包长度的数据类型
        constexpr uint PACKET_SIZE_TYPE_BYTES=sizeof(uint32);                   ///<描述包长度的数据类型的字节长度

        class SocketInputStream;
        class SocketOutputStream;

        /**
         * TCP服务器接入用户处理基类，为各种Server管理器提供统一调用接口<br>
         * *******************************************************************<br>
         * 收包和发包是可以在不同线程异步同时工作的，现在写一起只是暂时，未来分开
         */
        class TCPAccept:public TCPSocket
        {
        protected:

            SocketInputStream *sis=nullptr;
            SocketOutputStream *sos=nullptr;

            MemBlock<uchar> recv_buffer;

        protected://事件函数，由SocketManage调用

            friend class SocketManage;

            virtual int OnSocketRecv(int);                                      ///<Socket接收处理函数
            virtual int OnSocketSend(int);                                      ///<Socket发送处理函数
            virtual void OnSocketError(int);                                    ///<Socket错误处理函数

        public:

            TCPAccept();                                                        ///<本类构造函数
            TCPAccept(int,IPAddress *);                                         ///<本类构造函数
            virtual ~TCPAccept();

            virtual bool SendPacket(void *,const PACKET_SIZE_TYPE &);           ///<发包

            virtual bool OnRecvPacket(void *,const PACKET_SIZE_TYPE &)=0;       ///<接收包事件函数
        };//class TCPAccept:public TCPSocket
    }//namespace network
}//namespace hgl
#endif//HGL_NETWORK_TCP_ACCEPT_INCLUDE
