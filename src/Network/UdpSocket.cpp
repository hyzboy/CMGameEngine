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
			bind_addr=nullptr;
			tar_addr=nullptr;
		}

		/**
		* 本类析构函数
		*/
		UDPSocket::~UDPSocket()
		{
			CloseSocket();

			SAFE_CLEAR(tar_addr);
            SAFE_CLEAR(bind_addr);
		}

		/**
		* 创建一个UDP连接，使用指定的IP地址和端口
		* @param addr 地址
		* @return true 创建服务器成功
		* @return false 创建服务器失败
		*/
		bool UDPSocket::Create(const IPAddress *addr)
		{
            if(!addr)RETURN_FALSE;

            if(addr->GetSocketType()!=SOCK_DGRAM)RETURN_FALSE;
            if(addr->GetProtocol()!=IPPROTO_UDP)RETURN_FALSE;

			if(!CreateSocket(addr->GetFamily(),SOCK_DGRAM,IPPROTO_UDP))
				RETURN_FALSE;

			socket_protocols=IPPROTO_UDP;

            if(!addr->Bind(ThisSocket))
			{
				hgl::CloseSocket(ThisSocket);
				return(false);
			}

			SetBlock(false);
			return(true);
		}

		/**
		* 创建一个UDP连接
		*/
		bool UDPSocket::Create(int family)
		{
            if(family!=AF_INET&&family!=AF_INET6)RETURN_FALSE;

			if((ThisSocket=socket(family,SOCK_DGRAM,IPPROTO_UDP))<0)
			{
				LOG_ERROR(U16_TEXT("创建Socket失败！errno:")+UTF16String(GetLastSocketError()));
				return(false);
			}

			socket_protocols=IPPROTO_UDP;

			SetBlock(false);
			return(true);
		}

		/**
		* 设定发送数据时，接收数据方的地址
		* @param addr 接收方的地址
        * @return 是否成功
		*/
		bool UDPSocket::SetSendAddr(const IPAddress *addr)
		{
            if(ThisSocket==-1)RETURN_FALSE;

            SAFE_CLEAR(tar_addr);
			tar_addr=addr->CreateCopy();

            {
                const int opt = tar_addr->IsBoradcast()?1:0;

                if(setsockopt(ThisSocket, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt))==-1)
                    RETURN_FALSE;
            }

            return(true);
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
			return(sendto(ThisSocket,(char *)data,size,0,tar_addr->GetSockAddr(),tar_addr->GetSockAddrInSize()));
		}

		/**
		* 向指定地址发送数据包
		* @param addr 接收数据方的地址
		* @param data 数据指针
		* @param size 数据长度
		* @return 已发送的数据字节数
		*/
		int UDPSocket::SendPacket(IPAddress *addr,const void *data,int size)
		{
	#ifdef _DEBUG
			if(ThisSocket==-1)
			{
				LOG_HINT(U16_TEXT("UDPSocket没有调用Create"));
				return(-1);
			}
	#endif//
			return(sendto(ThisSocket,(char *)data,size,0,addr->GetSockAddr(),addr->GetSockAddrInSize()));
		}

		/**
		* 接收由外部发来的数据包
		* @param buf 接收用的数据缓冲区指针
		* @param size 缓冲区长度
		* @param remote_addr 发送方的地址
		* @return 接收到的数据长度
		*/
		int UDPSocket::RecvPacket(void *buf,int size,IPAddress *remote_addr)
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
			sas=remote_addr->GetSockAddrInSize();

			return(recvfrom(ThisSocket,(char *)buf,size,0,remote_addr->GetSockAddr(),&sas));
		}
	}//namespace network
}//namespace hgl

