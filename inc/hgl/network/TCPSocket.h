#ifndef HGL_TCP_SOCKET_INCLUDE
#define HGL_TCP_SOCKET_INCLUDE

#include<hgl/network/IOSocket.h>
#include<hgl/thread/RingBuffer.h>
namespace hgl
{
	class RingBuffer;
	template<typename T> class MemBlock;

	namespace network
	{
		int CreateTCPConnect(const sockaddr_in &);							///<创建一个tcp连接
		int CreateTCPConnect(const UTF8String &ip,const uint port);			///<创建一个tcp连接

		/**
		* TCP连接处理基类<br>
		* 提供统一的Recv/Send函数以及缓冲区，但请注意这个recv/send都只是针对缓冲区的，真正的send/recv在各自的派生类中。
		*/
		class TCPSocket:public IOSocket                                                               ///TCP连接处理基类
		{
		protected:

			sockaddr_in ThisAddr;																	///<当前socket对应的地址

			timeval time_out;
			fd_set local_set,recv_set,err_set;

			void InitPrivate();

		public: //被动事件函数

			virtual void ProcDisconnect()HGL_OVERRIDE{}													///<断线事件处理函数
			virtual int ProcRecv(int recv_buf_size=-1,const double cur_time=0)HGL_OVERRIDE				///<接收数据事件处理函数
						{return IOSocket::ProcRecv(recv_buf_size,cur_time);}
			virtual int ProcSend(int,int &left_bytes)HGL_OVERRIDE{return -1;}							///<发送数据事件处理函数

		public: //方法

			TCPSocket();                                                                            ///<本类构造函数
			TCPSocket(int);                                                                         ///<本类构造函数
			TCPSocket(int,const sockaddr_in *);														///<本类构造函数
			virtual ~TCPSocket();																	///<本类析构函数

			const sockaddr_in &GetAddr()const{return ThisAddr;}										///<取得当前socket地址
			bool SetNodelay(bool);																	///<设置是否使用无延迟方式

			virtual void UseSocket(int,const sockaddr_in *addr=0);									///<使用指定socket

			virtual bool IsConnect();																///<当前socket是否在连接状态
			virtual bool ReConnect();																///<重新连接

			virtual int WaitRecv(double);															///<等待可接收数据
		};//class TCPSocket

		/**
		 * 使用回呼事件机制的TCPSocket
		 */
		class TCPSocketCB:public TCPSocket
		{
		public:	//事件函数

			DefEvent(void,	OnDisconnect,	(TCPSocket *));											///<断线事件处理函数
			DefEvent(int,	OnRecv,			(TCPSocket *,int,const double ));						///<接收数据事件处理函数
			DefEvent(int,	OnSend,			(TCPSocket *,int,int &));								///<发送数据事件处理函数

		protected:

			void InitPrivate()
			{
				OnDisconnect=nullptr;
				OnRecv		=nullptr;
				OnSend		=nullptr;
			}

		public:

			TCPSocketCB(){InitPrivate();}															///<本类构造函数
			TCPSocketCB(int s):TCPSocket(s){InitPrivate();}											///<本类构造函数
			TCPSocketCB(int s,const sockaddr_in *addr):TCPSocket(s,addr){InitPrivate();}			///<本类构造函数
			virtual ~TCPSocketCB()HGL_DEFAULT_MEMFUNC;

			virtual void ProcDisconnect() HGL_OVERRIDE
			{
				SafeCallEvent(OnDisconnect,(this));
			}

			virtual int ProcRecv(int size,const double cur_time) HGL_OVERRIDE
			{
				if(OnRecv==nullptr)return(-1);

#ifdef HGL_VARIADIC_TEMPLATES
				return OnRecv(this,size,cur_time);
#else
				return CallEvent(OnRecv,(this,size,cur_time));
#endif//HGL_VARIADIC_TEMPLATES
			}

			virtual int ProcSend(int size,int &left_bytes) HGL_OVERRIDE
			{
				if(OnSend==nullptr)return(-1);

#ifdef HGL_VARIADIC_TEMPLATES
				return OnSend(this,size,left_bytes);
#else
				return CallEvent(OnSend,(this,size,left_bytes));
#endif//HGL_VARIADIC_TEMPLATES
			}
		};//class TCPSocketCB

		/**
		 * 带环形缓冲机制的TCPSocket<br>
		 * 这种模式的特点是数据收发不直接走recv/send，而是在一个缓存中。
		 */
		class TCPSocketRB:public TCPSocket
		{
		protected:

			io::RingInputStream *ris;
			io::RingOutputStream *ros;

			RingBuffer *RecvBuffer;
			RingBuffer *SendBuffer;

			MemBlock<char> *recv_temp_buffer;
			char *send_temp_buffer;

		protected:

			virtual void InitPrivate();

		protected:

			virtual int ProcRecv(int=-1,const double cur_time=0)HGL_OVERRIDE;						///<接收数据处理(被回调函数)

		public:

			virtual int ProcSend(int,int &left_bytes)HGL_OVERRIDE;									///<发送数据处理(用户主动调用函数)

		public:

			TCPSocketRB();																			///<本类构造函数
			TCPSocketRB(int s);																		///<本类构造函数
			TCPSocketRB(int s,const sockaddr_in *addr);												///<本类构造函数
			virtual ~TCPSocketRB();

			virtual void UseSocket(int,const sockaddr_in *addr=0);									///<使用指定socket
			virtual io::InputStream *GetInputStream(){return ris;}									///<取得输入流
			virtual io::OutputStream *GetOutputStream(){return ros;}								///<取得输出流

			const	int	GetLeftSendBytes()const HGL_OVERRIDE{return left_send_bytes;}				///<取得剩余可发送数据
		};//class TCPSocketRB
	}//namespace network

	using namespace network;
}//namespace hgl
#endif//HGL_TCP_SOCKET_INCLUDE
