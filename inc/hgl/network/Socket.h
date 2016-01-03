#ifndef HGL_SOCKET_INCLUDE
#define HGL_SOCKET_INCLUDE

#include<hgl/type/DataType.h>
#include<hgl/type/BaseString.h>
#include<hgl/Str.h>

#if HGL_OS == HGL_OS_Windows
	#include<winsock2.h>
	#include<ws2tcpip.h>

	#if SOMAXCONN == 5
	#error Please use <winsock2.h>
	#endif//

	typedef int socklen_t;
	typedef ULONG in_addr_t;

	#define GetLastSocketError() WSAGetLastError()
#else
	#include<errno.h>
	#include<sys/types.h>
	#include<sys/ioctl.h>
	#include<sys/socket.h>
	#include<unistd.h>
	#include<netdb.h>
	#include<arpa/inet.h>
	#include<netinet/in.h>

	#define GetLastSocketError() (errno)

	#if HGL_OS == HGL_OS_Linux
		#include<sys/sendfile.h>

		inline int sendfile(int tfd,int sfd,size_t size)
		{
			return sendfile(tfd,sfd,nullptr,size);
		}
	#endif//HGL_OS == HGL_OS_Linux

	#if HGL_OS == HGL_OS_FreeBSD
		#include<sys/uio.h>

		inline int sendfile(int tfd,int sfd,size_t size)
		{
			return sendfile(tfd,sfd,0,size,nullptr,nullptr,0);
		}
	#endif//HGL_OS == HGL_OS_FreeBSD
#endif//HGL_OS == HGL_OS_Windows

//#define HGL_RECV_BYTE_COUNT			///<接收字节数统计(调试用)
//#define HGL_SEND_BYTE_COUNT			///<发送字节数统计(调试用)

//#define HGL_SOCKET_SEND_LIMIT_SIZE	///<发送限制包尺寸,默认为不限制

#include<hgl/network/ip_tool.h>

namespace hgl
{
	namespace io
	{
		class DataInputStream;
		class DataOutputStream;
	}//namespace io

	namespace network																					///网络相关处理模块名字空间
	{
        #define HGL_IPV4_STRING_MAX         21          ///<IPv4字符串最大长度(255.255.255.255:65535)

		#define HGL_NETWORK_MAX_PORT		65535		///<最大端口号
		#define HGL_NETWORK_IPv4_STR_MIN	7			///<IPv4字符串最小字符数

		enum SocketError
		{
			nseNoError      =0,         ///<没有错误

			nseInt			=4,			///<系统中断呼叫，一般出现在Debug时的人工中断
			nseIOError		=5,			///<I/O错误

			nseTryAgain		=11,		///<请再次尝试
			nseTooManyLink	=24,		///<太多连接

			nseBrokenPipe	=32,		///<管道破裂，一般是因为发送到一半对方断开所引起

			nseClientBreak	=10053,		///<客户端主动断开
			nseServerBreak,				///<服务器端主动断开

#if HGL_OS == HGL_OS_Windows
			nseTimeOut		=10060,		///<超时
#else
			nseTimeOut		=ETIMEDOUT,	///<超时
#endif//
		};//enum SocketError

		/**
		* Raw Packet length:
		*	UDP: MTU - IPHeader(20) - UDPHeader(8)
		*	TCP: MTU - IPHeader(20) - TCPHeader(20)
		*
		* Normal MUT:
		*	1500 (Ethernet, DSL broadband)
		*	1492 (PPPoE broadband)
		*	576 (dial-up)
		*/

		#define HGL_SERVER_LISTEN_COUNT		SOMAXCONN												///<Server最大监听数量
		//Linux 是 128
		//winsock 1.x SOMAXCONN是5
		//winsock 2.x SOMAXCONN是0x7fffffff

		#define HGL_NETWORK_TIME_OUT		HGL_TIME_ONE_MINUTE										///<默认超时时间
		#define HGL_NETWORK_HEART_TIME		(HGL_NETWORK_TIME_OUT/2)								///<默认心跳时间(注：心跳并不是每隔指定时间都发送，而是离上一次发送任意封包超过指定时间才发送)
		#define HGL_NETWORK_DOUBLE_TIME_OUT	(HGL_NETWORK_TIME_OUT*2)								///<2次超时时间

		#define HGL_SERVER_OVERLOAD_RESUME_TIME	10													///<服务器超载再恢复等待时间

		#define HGL_TCP_BUFFER_SIZE			HGL_SIZE_1KB*256										///<TCP缓冲区大小

		typedef  int32 HGL_PACKET_SIZE;																///<包长度数据类型定义
		typedef uint32 HGL_PACKET_TYPE;																///<包类型数据类型定义

		#define HGL_PACKET_SIZE_BYTES		sizeof(HGL_PACKET_SIZE)									///<包长度数据类型字节数
		#define HGL_PACKET_TYPE_BYTES		sizeof(HGL_PACKET_TYPE)									///<包类型数据类型字节数
		#define HGL_PACKET_HEADER_BYTES		(HGL_PACKET_SIZE_BYTES+HGL_PACKET_TYPE_BYTES)			///<包头数据类型字节数

		class Socket;
		class IOSocket;
	}//namespace network

	namespace network
	{
		bool GetHostname(UTF8String &hostname);														///<取得本机主机名称

		void CloseSocket(int);																		///<关闭socket
		void SetSocketBlock(int ThisSocket,bool block,double send_time_out=HGL_NETWORK_TIME_OUT,
													double recv_time_out=HGL_NETWORK_TIME_OUT);		///<设置socket是否使用阻塞方式

		void SetSocketLinger(int ThisSocket,int time_out);											///<设置Socket关闭时的确认数据发送成功超时时间

		const os_char *GetSocketString(int);

		#define GetLastSocketErrorString() GetSocketString(GetLastSocketError())

		/**
		* 将一个IPv4 地址转换成字符串
		* @param addr 要转换的地址32位数
		* @param name 转换后的域名或IP字符串,请不要少于20个字节(ex:123.456.789.012:65535)
		* @param port 是否包含port
		*/
		template<typename T>
		void SockToStr(const uint32 addr,const uint16 port,T name[HGL_IPV4_STRING_MAX+1],bool inc_port)
		{
			uint8 *p=(uint8 *)&addr;

			T str[8];

			utos(str,8,*p++);
			strcpy(name,3,str);
			strcat(name,4,(T)'.');

			utos(str,8,*p++);
			strcat(name,HGL_IPV4_STRING_MAX,str,3);
			strcat(name,8,(T)'.');

			utos(str,8,*p++);
			strcat(name,HGL_IPV4_STRING_MAX,str,3);
			strcat(name,12,(T)'.');

			utos(str,8,*p);
			strcat(name,HGL_IPV4_STRING_MAX,str,3);

			if(inc_port)
			{
				strcat(name,16,(T)':');
			#if HGL_ENDIAN == HGL_LITTLE_ENDIAN
				utos(str,8,((port&0xFF00)>>8)|((port&0xFF)<<8));		//port永远为big endian,即使os/cpu为little endian,不过似乎win下会转换一下
			#else
				utos(str,8,port);
			#endif//little endian
				strcat(name,HGL_IPV4_STRING_MAX,str,5);
			}
		}

		/**
		* 将一个IPv4 SockAddr转换成字符串
		* @param sock 要转换的SockAddr结构
		* @param name 转换后的域名或IP字符串,请不要少于20个字节(ex:123.456.789.012:65535)
		* @param port 是否包含port
		*/
		template<typename T>
		void SockToStr(const sockaddr_in &sock,T *name,bool port)
		{
			return SockToStr<T>(sock.sin_addr.s_addr,sock.sin_port,name,port);
		}

		bool Read(io::DataInputStream *dis,sockaddr_in &addr);
		bool Write(io::DataOutputStream *dos,const sockaddr_in &addr);

		/**
		* 所有Socket通信类的基类
		*/
		class Socket                                                                                    ///Socket基类
		{
			friend class SocketManage;

		protected:

			int socket_domain;																			///<Socket域
			int socket_type;																			///<Socket类型
			int socket_protocols;																		///<Socket协议

			bool CreateSocket(int,int,int);																///<创建Socket

			bool bindaddr(const sockaddr_in &);															///<绑定指定域名/IP和PORT到当前socket
			bool bindaddr(const char *,int);                                                            ///<绑定指定域名/IP和PORT到当前socket

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

		public: //被动事件函数

			virtual void	ProcDisconnect()=0;															///<断线事件处理函数
		};//class Socket
	}//namespace network

	using namespace network;
}//namespace hgl
#endif//HGL_SOCKET_INCLUDE
