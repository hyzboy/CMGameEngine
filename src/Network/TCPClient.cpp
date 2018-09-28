#include<hgl/network/TCPClient.h>
#include<hgl/LogInfo.h>
#include<hgl/network/SocketInputStream.h>
#include<hgl/network/SocketOutputStream.h>

/*
发送数据

模式一：
        理论：主线程每循环结束时 主动进行 接收缓冲区锁定、发送缓冲区锁定，如锁定成功，下一循环开始时将其提交到发送线程。
        缺点：虽是多线程，但利用率不高，缓冲区仍是独占，而且长期独占

模式二：
        理论：每次发包时锁定缓冲区，发完解锁。
        缺点：过于频繁加锁解锁，影响整体程序效能。

理想模式：
        独立的双缓冲区：主线程每次写入的为后备缓冲区，当每循环刷新或是缓冲区满时，锁定缓冲区，与发送线程交换或是复制到发送线程缓冲区。
*/

/*
接收

接收线程模式（现有）：
        理论：接收线程独立一个暂存缓冲区，先从socket接收到暂存缓冲区，然后锁定主缓冲区，复制完成后解锁。

主线程模式（旧方案）：
        理论：每次读包时，锁定缓冲区，读完解锁。
        缺点：过于频繁加锁解锁，影响整体程序效能。

主线程模式（备选方案）：
        理论：每次访问开始锁定缓冲区，完全读走所有包后，再解锁。
        缺点：长期锁定主缓冲区，会造成接收线程收到数据后无法写入，长时间会造成socket接收缓冲区暴满。
*/

namespace hgl
{
    namespace network
    {
        void TCPClient::InitPrivate()
        {
            TimeOut=HGL_NETWORK_TIME_OUT;

            sis=new SocketInputStream();
            sos=new SocketOutputStream();

            ipstr = nullptr;
        }

        /**
        * TCP客户端连接类构造函数
        */
        TCPClient::TCPClient():TCPSocket()
        {
            InitPrivate();
        }

        TCPClient::TCPClient(int sock,const IPAddress *addr):TCPSocket(sock,addr)
        {
            InitPrivate();
        }

        TCPClient::~TCPClient()
        {
            Disconnect();
            SAFE_CLEAR(sis);
            SAFE_CLEAR(sos);
            SAFE_CLEAR_ARRAY(ipstr);
        }

        /**
         * 连接到服务器
         * @return 是否连接成功
         */
        bool TCPClient::Connect()
        {
            if(ThisSocket<0||!ThisAddress)RETURN_FALSE;

            if(connect(ThisSocket,ThisAddress->GetSockAddr(),ThisAddress->GetSockAddrInSize()))
            {
                SAFE_CLEAR(ipstr);
                ipstr=ThisAddress->CreateString();

                LOG_HINT(U8_TEXT("Don't Connect to TCPServer ")+UTF8String(ipstr));
                this->CloseSocket();
                return(false);
            }

//            LOG_INFO(u"connect "+u8_to_u16(host)+u':'+UTF16String(port)+u" ok,socket:"+UTF16String(ThisSocket));

            SetBlock(true,TimeOut);    //阻塞模式

            ResetConnect();

            return(true);
        }

        /**
         * 创建一个连接
         * @param addr 服务器地址
         */
        bool TCPClient::CreateConnect(const IPAddress *addr)
        {
            if(!addr)
                RETURN_FALSE;

            if(!addr->IsTCP())
                RETURN_FALSE;

            if(!InitSocket(addr))
                RETURN_FALSE;

            return this->Connect();
        }

        /**
        * 与服务器断开
        */
        void TCPClient::Disconnect()
        {
            SAFE_CLEAR_ARRAY(ipstr);

            if(ThisSocket==-1)
                return;

            this->CloseSocket();            //两个线程会自动因为socket关闭而退出
                                            //注：线程退出会调用ThreadExit，再次调用Disconnect
        }

        /**
         * 使用指定socket
         * @param sock 指定socket编号
         * @param addr socket地址
         */
        bool TCPClient::UseSocket(int sock,const IPAddress *addr)
        {
            if(!TCPSocket::UseSocket(sock,addr))
                RETURN_FALSE;

            ((SocketInputStream *)sis)->SetSocket(sock);
            ((SocketOutputStream *)sos)->SetSocket(sock);

            return(true);
        }

        TCPClient *CreateTCPClient(IPAddress *addr)
        {
            if(!addr)RETURN_ERROR_NULL;

            if(!addr->IsTCP())
                RETURN_ERROR_NULL;

            int sock=CreateSocket(addr);

            if(sock<0)
                RETURN_ERROR_NULL;

            if(!Connect(sock,addr))
            {
                CloseSocket(sock);
                RETURN_ERROR_NULL;
            }

            return(new TCPClient(sock,addr));
        }
    }//namespace network
}//namespace hgl
