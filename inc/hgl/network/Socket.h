#ifndef HGL_SOCKET_INCLUDE
#define HGL_SOCKET_INCLUDE

#include<hgl/network/IP.h>

//#define HGL_RECV_BYTE_COUNT			///<接收字节数统计(调试用)
//#define HGL_SEND_BYTE_COUNT			///<发送字节数统计(调试用)

//#define HGL_SOCKET_SEND_LIMIT_SIZE	///<发送限制包尺寸,默认为不限制

namespace hgl
{
	namespace io
	{
		class DataInputStream;
		class DataOutputStream;
	}//namespace io

	namespace network																					///网络相关处理模块名字空间
	{
		constexpr uint HGL_NETWORK_MAX_PORT		=65535;		///<最大端口号
		constexpr uint HGL_NETWORK_IPv4_STR_MIN	=7;			///<IPv4字符串最小字符数(0.0.0.0)
		constexpr uint HGL_NETWORK_IPV4_STR_MAX =21;        ///<IPv4字符串最大长度(255.255.255.255:65535)

		enum SocketError
		{
			nseNoError      =0,         ///<没有错误

			nseInt			=4,			///<系统中断呼叫，一般出现在Debug时的人工中断
			nseIOError		=5,			///<I/O错误

			nseTryAgain		=11,		///<请再次尝试
			nseTooManyLink	=24,		///<太多连接

			nseBrokenPipe	=32,		///<管道破裂，一般是因为发送到一半对方断开所引起

#if HGL_OS == HGL_OS_Windows
            nseWouldBlock   =WSAEWOULDBLOCK,
			nseSoftwareBreak=WSAECONNABORTED,    ///<我方软件主动断开
			nsePeerBreak    =WSAECONNRESET,      ///<对方主动断开
			nseTimeOut		=WSAETIMEDOUT,       ///<超时
#else
            nseWouldBlock   =EWOULDBLOCK,
			nseSoftwareBreak=ECONNABORTED,       ///<我方软件主动断开
			nsePeerBreak    =ECONNRESET,         ///<对方主动断开
			nseTimeOut		=ETIMEDOUT,          ///<超时
#endif//
		};//enum SocketError

		/**
		* Raw Packet length:
		*	UDP: MTU - IPHeader(20) - UDPHeader(8)
		*	TCP: MTU - IPHeader(20) - TCPHeader(20)
		*
		* Normal MTU:
		*	1500 (Ethernet, DSL broadband)
		*	1492 (PPPoE broadband)
		*	576 (dial-up)
		*/

        constexpr uint HGL_SERVER_LISTEN_COUNT		   =SOMAXCONN;									///<Server最大监听数量
		//Linux 是 128
		//winsock 1.x SOMAXCONN是5
		//winsock 2.x SOMAXCONN是0x7fffffff

		constexpr uint HGL_NETWORK_TIME_OUT		       =HGL_TIME_ONE_MINUTE;						///<默认超时时间
		constexpr uint HGL_NETWORK_HEART_TIME		   =HGL_NETWORK_TIME_OUT/2;						///<默认心跳时间(注：心跳并不是每隔指定时间都发送，而是离上一次发送任意封包超过指定时间才发送)
		constexpr uint HGL_NETWORK_DOUBLE_TIME_OUT	   =HGL_NETWORK_TIME_OUT*2;						///<2次超时时间

        constexpr uint HGL_SERVER_OVERLOAD_RESUME_TIME =10;											///<服务器超载再恢复等待时间

        constexpr uint HGL_TCP_BUFFER_SIZE			   =HGL_SIZE_1KB*256;							///<TCP缓冲区大小

		typedef  int32 HGL_PACKET_SIZE;																///<包长度数据类型定义
		typedef uint32 HGL_PACKET_TYPE;																///<包类型数据类型定义

		constexpr uint HGL_PACKET_SIZE_BYTES		=sizeof(HGL_PACKET_SIZE);						///<包长度数据类型字节数
        constexpr uint HGL_PACKET_TYPE_BYTES		=sizeof(HGL_PACKET_TYPE);						///<包类型数据类型字节数
        constexpr uint HGL_PACKET_HEADER_BYTES		=HGL_PACKET_SIZE_BYTES+HGL_PACKET_TYPE_BYTES;	///<包头数据类型字节数

		class Socket;
		class IOSocket;

#if HGL_OS == HGL_OS_Windows
    bool InitWinSocket();
#endif//HGL_OS == HGL_OS_Windows
	}//namespace network

	namespace network
	{
		void CloseSocket(int);																		///<关闭socket
		void SetSocketBlock(int ThisSocket,bool block,double send_time_out=HGL_NETWORK_TIME_OUT,
													double recv_time_out=HGL_NETWORK_TIME_OUT);		///<设置socket是否使用阻塞方式

		void SetSocketLinger(int ThisSocket,int time_out);											///<设置Socket关闭时的确认数据发送成功超时时间

		const os_char *GetSocketString(int);

		#define GetLastSocketErrorString() GetSocketString(GetLastSocketError())

		bool Read(io::DataInputStream *dis,IPAddress *addr);
		bool Write(io::DataOutputStream *dos,const IPAddress *addr);

		/**
		* 所有Socket通信类的基类
		*/
		class Socket                                                                                    ///Socket基类
		{
		protected:

			int socket_domain;																			///<Socket域
			int socket_type;																			///<Socket类型
			int socket_protocols;																		///<Socket协议

			bool CreateSocket(int,int,int);																///<创建Socket

		public: //属性

			int	ThisSocket;																				///<当前socket编号

		public: //方法

			Socket();
			virtual ~Socket();

			const	int		GetSocketDomain()const{return socket_domain;}								///<取得Socket
			const	int		GetSocketType()const{return socket_type;}									///<取得Socket类型
			const	int		GetSocketProtocols()const{return socket_protocols;}							///<取得Socket协议

			virtual void	CloseSocket();																///<关闭连接

					/**
					* 设置是否使用堵塞方式传输
					* @param block 是否使用堵塞方式(true/false)
					* @param sto 发送数据超时时间(单位: 秒)
					* @param rto 接收数据超时时间(单位: 秒)
					*/
					void	SetBlock(bool block,double sto=HGL_NETWORK_TIME_OUT,
												double rto=HGL_NETWORK_TIME_OUT)						///<设置是否使用堵塞方式
					{
						SetSocketBlock(ThisSocket,block,sto,rto);
					}

// 		public: //被动事件函数
//
// 			virtual void	ProcDisconnect()=0;															///<断线事件处理函数
		};//class Socket
	}//namespace network

	using namespace network;
}//namespace hgl
#endif//HGL_SOCKET_INCLUDE
