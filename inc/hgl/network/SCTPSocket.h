#ifndef HGL_NETWORK_SCTP_SOCKET_INCLUDE
#define HGL_NETWORK_SCTP_SOCKET_INCLUDE

#include<hgl/network/Socket.h>
#include<hgl/MemBlock.h>
#include<netinet/sctp.h>
namespace hgl
{
	namespace network
	{
		/**
		 * sctp socket功能基类
		 */
		class SCTPSocket:public Socket
		{
		protected:

			int out_max_stream;
			int in_max_stream;
			int init_max_attempts;
			int init_max_init_timeo;

			sockaddr_in address;

			bool GetMsg();
			bool InitDataIOEvent();
			bool InitMsg(int out_stream=0,int in_stream=0,int attempts=0,int init_time_out=0);

		protected:

			void ProcDisconnect()=default;
			int ProcRecv(int)=default;
			int ProcSend(int,int &)=default;

		public:

			virtual	bool SetNodelay(bool);															///<设置是否使用无延迟方式
			virtual bool SetMaxStream(int);															///<设置最大流数

		public:

			SCTPSocket();
			virtual ~SCTPSocket()=default;

			virtual void UseSocket(int,const sockaddr_in *addr=nullptr);							///<使用指定socket
		};//class SCTPSocket

		class SCTPO2OSocket:public SCTPSocket
		{
			struct sctp_sndrcvinfo sri_recv;
			struct sctp_sndrcvinfo sri_send;

		public:

			using SCTPSocket::SCTPSocket;
			virtual ~SCTPO2OSocket()=default;

			virtual bool SendMsg(const void *,int len,uint16 stream);								///<发送一个数据包
			virtual bool RecvMsg(MemBlock<char> *,uint16 &stream);									///<接收一个数据包
		};

		class SCTPO2MSocket:public SCTPSocket
		{
		public:

			using SCTPSocket::SCTPSocket;
			virtual ~SCTPO2MSocket()=default;

			virtual bool Create();																	///<创建一个Socket

			virtual bool SendMsg(const sockaddr_in *,void *,int len,uint16);						///<发送一个数据包
			virtual bool RecvMsg(sockaddr_in &,void *,int max_len,int &len,uint16 &);				///<接收一个数据包
		};//
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SCTP_SOCKET_INCLUDE
