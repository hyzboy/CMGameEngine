//--------------------------------------------------------------------------------------------------
//《古月游戏开发库v18》UDP通信组件类源程序文件 UDPSocket.CPP
//--------------------------------------------------------------------------------------------------
#include <hgl/LogInfo.h>
#include <hgl/network/UdpSocket.h>
#include <string.h>

#if HGL_OS != HGL_OS_Windows
#include<netinet/udp.h>
#endif//HGL_OS != HGL_OS_Windows
//--------------------------------------------------------------------------------------------------
namespace hgl
{
	namespace network
	{
		/**
		* 本类构造函数
		*/
		UDPSocket::UDPSocket()
		{
			ThisSocket=-1;
			bind_port=0;
			tar_addr=new sockaddr_in;
		}

		/**
		* 本类析构函数
		*/
		UDPSocket::~UDPSocket()
		{
			CloseSocket();

			delete tar_addr;
		}

		/**
		* 创建一个UDP连接，使用指定的IP地址和端口
		* @param host 服务器所使用的IP地址,如果host==nullptr则表示使用本机的所有地址
		* @param port 服务器所需监听的端口
		* @return true 创建服务器成功
		* @return false 创建服务器失败
		*/
		bool UDPSocket::Create(const char *host,const uint port)
		{
			if(!CreateSocket(AF_INET,SOCK_DGRAM,IPPROTO_UDP))
				return(false);

			socket_protocols=IPPROTO_UDP;

			if(!BindAddr(ThisSocket,host,port))
			{
				hgl::CloseSocket(ThisSocket);
				return(false);
			}

			bind_port=port;

			SetBlock(false);
			return(true);
		}

		/**
		* 创建一个UDP连接
		*/
		bool UDPSocket::Create()
		{
			if((ThisSocket=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP))<0)
			{
				LOG_ERROR(U16_TEXT("创建Socket失败！errno:")+UTF16String(GetLastSocketError()));
				return(false);
			}

			socket_protocols=IPPROTO_UDP;

			SetBlock(false);
			return(true);
		}

		/**
		* 设定发送数据时，接收数据方的IP地址和端口号
		* @param host 接收方的主机或IP地址
		* @param port 接收方的端口号
		*/
		void UDPSocket::SetSendAddr(const char *host,const uint port)
		{
			FillAddr(tar_addr,host,port);
		}

		/**
		* 设定发送数据时，接收数据方的地址
		* @param addr 接收方的地址
		*/
		void UDPSocket::SetSendAddr(sockaddr_in &addr)
		{
			memcpy(tar_addr,&addr,sizeof(sockaddr_in));
		}

		/**
		* 向已指定的地址和端口发送数据包
		* @param data 数据指针
		* @param size 数据长度
		* @return 已发送的数据字节数
		*/
		int UDPSocket::SendPacket(const void *data,int size)
		{
	#ifdef _DEBUG
			if(ThisSocket==-1)
			{
				LOG_HINT(U16_TEXT("UDPSocket没有调用Create"));
				return(-1);
			}
	#endif//
			return(sendto(ThisSocket,(char *)data,size,0,(sockaddr *)tar_addr,sizeof(sockaddr_in)));
		}

		/**
		* 向指定地址发送数据包
		* @param addr 接收数据方的地址
		* @param data 数据指针
		* @param size 数据长度
		* @return 已发送的数据字节数
		*/
		int UDPSocket::SendPacket(sockaddr_in &addr,const void *data,int size)
		{
	#ifdef _DEBUG
			if(ThisSocket==-1)
			{
				LOG_HINT(U16_TEXT("UDPSocket没有调用Create"));
				return(-1);
			}
	#endif//
			return(sendto(ThisSocket,(char *)data,size,0,(sockaddr *)&addr,sizeof(sockaddr_in)));
		}

		/**
		* 接收由外部发来的数据包
		* @param buf 接收用的数据缓冲区指针
		* @param size 缓冲区长度
		* @param remote_addr 发送方的地址
		* @return 接收到的数据长度
		*/
		int UDPSocket::RecvPacket(void *buf,int size,sockaddr_in *remote_addr)
		{
	#ifdef _DEBUG
			if(ThisSocket==-1)
			{
				LOG_HINT(U16_TEXT("UDPSocket没有调用Create"));
				return(-1);
			}
	#endif//

			static

	#if HGL_OS == HGL_OS_Windows
			int
	#else
			socklen_t
	#endif//
			sas=sizeof(sockaddr_in);

			return(recvfrom(ThisSocket,(char *)buf,size,0,(sockaddr *)remote_addr,&sas));
		}
	}//namespace network
}//namespace hgl

