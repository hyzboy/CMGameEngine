#ifndef HGL_NETWORK_IP_TOOL_INCLUDE
#define HGL_NETWORK_IP_TOOL_INCLUDE

#include<hgl/type/List.h>
namespace hgl
{
	namespace network
	{
		template<int FAMILY,typename InAddr, typename SockAddr, typename SockAddrIn>
		struct ip_tool
		{
		private:

			static void AddAddrToList(List<InAddr> &addr_list, const SockAddrIn *sai);

			static int GetIPList(const char *addr_string, List<InAddr> &addr_list)
			{
				struct addrinfo hints, *answer, *ptr;

				hgl_zero(hints);
				hints.ai_family = FAMILY;

				if (getaddrinfo(addr_string, nullptr, &hints, &answer))			//此函数最低Windows 2003/Vista
					return(-1);

				int count = 0;
				for (ptr = answer; ptr; ptr = ptr->ai_next)
				{
					AddAddrToList(addr_list,(SockAddrIn *)(ptr->ai_addr));
					++count;
				}

				return(count);
			}

		public:

			/**
			* 取得本机IP地址
			* @param addr_list 本机IP地址列表
			* @return 本机IP地址数量,-1表示失败
			*/
			static int	GetLocalIP(List<InAddr> &addr_list)											///<取得本机IP地址列表
			{
				return GetIPList("localhost", addr_list);
			}

			/**
			* 将一个域名转换成IP(IP格式)
			* @param domain 域名
			* @param addr_list IP地址列表
			* @return IP地址数量,-1表示失败
			*/
			static int	Domain2IP(const UTF8String &domain,List<InAddr> &addr_list)					///<转换域名到IP地址
			{
				if (name.Length() <= 0)return(-1);

				return GetIPList(name.c_str(), addr_list);
			}

			/**
			* 绑定指定域名/IP和port到当前socket
			* @param ThisSocket 要指定的当前socket
			* @param addr 指定的ip/port
			* @param reuse 是否复用当前地址,默认为1
			* @return 是否成功
			*/
			static bool	BindAddr(int ThisSocket,const SockAddrIn &addr,int reuse=1)
			{
				const int val = reuse;

#if HGL_OS == HGL_OS_Windows				
				setsockopt(ThisSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&val, sizeof(BOOL));		//win下的BOOL本质也是int，所以唯一区分只在于val的传入类型
#else
				setsockopt(ThisSocket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int));
#endif//HGL_OS == HGL_OS_Windows

				if (bind(ThisSocket, (SockAddr *)&addr, sizeof(SockAddrIn)))
				{
					LOG_ERROR(OS_TEXT("Bind Socket Error! errno: ") + OSString(GetLastSocketError()));
					std::cerr << "Bind Socket Error! errno: " << GetLastSocketError() << std::endl;
					return(false);
				}

				return(true);
			}

			static bool FillAddr(SockAddrIn *addr, const UTF8String &addr_string, ushort port)		///<将指定域名或地址填充到sockaddr_in结构中
			{

			}

			static bool	BindAddr(int ThisSocket, const char *name, int port)
			{
				SockAddrIn addr;

				if (!FillAddr(&addr, name, port))return(false);

				return BindAddr(ThisSocket, addr);
			}

			/**
			* 填充供服务器使用的任意地址项到结构
			*/
			static void FillAddrByAny(SockAddrIn &addr, ushort port);
		};

		using ipv4 = ip_tool<AF_INET,	struct in_addr,	struct sockaddr,	struct sockaddr_in	>;
		using ipv6 = ip_tool<AF_INET6,	struct in6_addr,struct sockaddr6,	struct sockaddr_in6	>;

		template<> void ipv4::AddAddrToList(List<in_addr> &addr_list, const sockaddr_in *sai)
		{
			addr_list.Add(sai->sin_addr);
		}
		
		template<> void ipv6::AddAddrToList(List<in6_addr> &addr_list, const sockaddr_in6 *sai)
		{
			addr_list.Add(sai->sin6_addr);
		}

		template<> void ipv4::FillAddrByAny(struct sockaddr_in &addr, ushort port)
		{
			hgl_zero(addr);

			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
		}

		template<> void ipv6::FillAddrByAny(struct sockaddr_in6 &addr, ushort port)
		{
			hgl_zero(addr);

			addr.sin6_family = AF_INET6;
			addr.sin6_port = htons(port);
		}
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_IP_TOOL_INCLUDE
