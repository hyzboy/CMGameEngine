#ifndef HGL_TCP_CLIENT_INCLUDE
#define HGL_TCP_CLIENT_INCLUDE

#include<hgl/network/TCPSocket.h>
#include<hgl/io/InputStream.h>
#include<hgl/io/OutputStream.h>
namespace hgl
{
    namespace network
    {
        /**
        * TCP客户端处理类，程序模式为多线程阻塞模式。该类会创建2个独立的线程分别用于收包和发包。
        */
        class TCPClient:public TCPSocket                                                                ///TCP客户端处理类
        {
            io::InputStream *sis;
            io::OutputStream *sos;

            char *ipstr;

            virtual void InitPrivate(int);

        public:

            double Heart;                                                                               ///<心跳间隔时间(单位:秒，默认参见HGL_TCP_HEART_TIME)
            double TimeOut;                                                                             ///<超时时间(单位:秒，默认参见HGL_NETWORK_TIME_OUT)

        public:

            TCPClient();                                                                                ///<本类构造函数
            TCPClient(int,const IPAddress *);                                                           ///<本类构造函数
            virtual ~TCPClient();                                                                       ///<本类析构函数

            virtual bool Connect();                                                                     ///<连接到服务器
            virtual bool CreateConnect(const IPAddress *);                                              ///<创建一个连接
            virtual void Disconnect();                                                                  ///<断开连接
            virtual bool UseSocket(int,const IPAddress *addr) override;                                 ///<使用指定socket

            const char *GetIPString()const{return ipstr;}                                               ///<取得IP可视字符串

        public:

            virtual io::InputStream *GetInputStream(){return sis;}                                      ///<取得输入流
            virtual io::OutputStream *GetOutputStream(){return sos;}                                    ///<取得输出流
        };//class TCPClient

        TCPClient *CreateTCPClient(IPAddress *);
    }//namespace network

    using namespace network;
}//namespace hgl
#endif//HGL_TCP_CLIENT_INCLUDE
