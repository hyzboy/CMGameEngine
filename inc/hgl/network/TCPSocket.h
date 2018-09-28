#ifndef HGL_TCP_SOCKET_INCLUDE
#define HGL_TCP_SOCKET_INCLUDE

#include<hgl/network/Socket.h>
namespace hgl
{
	namespace network
	{
		int CreateTCPConnect(IPAddress *);							                                ///<创建一个tcp连接

		/**
		* TCP连接处理基类<br>
		* 提供统一的Recv/Send函数以及缓冲区，但请注意这个recv/send都只是针对缓冲区的，真正的send/recv在各自的派生类中。
		*/
		class TCPSocket:public Socket                                                               ///TCP连接处理基类
		{
		protected:

			timeval time_out;
			fd_set local_set,recv_set,err_set;

            void ResetConnect();

		public: //方法

            using Socket::Socket;
            TCPSocket(int sock,const IPAddress *addr):Socket(sock,addr){ResetConnect();}
			virtual ~TCPSocket()=default;															///<本类析构函数

			bool SetNodelay(bool);																	///<设置是否使用无延迟方式
			void SetKeepAlive(bool,const int=7200,const int=75,const int=9);						///<设置自动保持连接机制

            virtual bool UseSocket(int,const IPAddress *) override;                                 ///<使用指定socket

			virtual bool IsConnect();																///<当前socket是否在连接状态

			virtual int WaitRecv(double);															///<等待可接收数据
		};//class TCPSocket
	}//namespace network

	using namespace network;
}//namespace hgl
#endif//HGL_TCP_SOCKET_INCLUDE
