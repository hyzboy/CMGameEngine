#include<hgl/network/TCPSocket.h>

#if HGL_OS != HGL_OS_Windows
#include<netinet/tcp.h>
#endif//HGL_OS != HGL_OS_Windows

namespace hgl
{
	void SetTimeVal(timeval &tv,const double t_sec);

	namespace network
	{
		/**
		 * 创建一个TCP连接
		 */
		int CreateTCPConnect(const sockaddr_in &addr)
		{
			int sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

			if(sock<0)return(-1);

			if(connect(sock,(sockaddr *)&addr,sizeof(addr)))
			{
				CloseSocket(sock);
				return(-1);
			}

			return sock;
		}

		/**
		 * 创建一个TCP连接
		 */
		int CreateTCPConnect(const UTF8String &ip,const uint port)
		{
			sockaddr_in addr;

			if(!FillAddr(&addr,ip,port))
				return(false);

			return CreateTCPConnect(addr);
		}
	}//namespace network

	namespace network
	{
		void TCPSocket::InitPrivate()
		{
			socket_protocols=IPPROTO_TCP;
		}

		/**
		* 构造函数
		*/
		TCPSocket::TCPSocket()
		{
			InitPrivate();
		}

		/**
		* 构造函数
		* @param sock Socket号
		*/
		TCPSocket::TCPSocket(int sock)
		{
			InitPrivate();

			UseSocket(sock);
		}

		/**
		* 构造函数
		* @param sock socket号
		* @param addr socket地址
		*/
		TCPSocket::TCPSocket(int sock,const sockaddr_in *addr)
		{
			InitPrivate();

			UseSocket(sock,addr);
		}

		TCPSocket::~TCPSocket()
		{
		}

		/**
		* 设置是否使用无延迟模式(注：无延迟模式也无缓冲，有利于小包高响应应用，大包或无高响应需要求应用不要设置)
		* @param no_delay 是否无延迟
		*/
		bool TCPSocket::SetNodelay(bool no_delay)
		{
			int flag=(no_delay?1:0);

			return(setsockopt(ThisSocket,IPPROTO_TCP,TCP_NODELAY,(char *)&flag,sizeof(flag))!=-1);
		}

		/**
		 * 设置是否使唤用KEEPALIVE自动保持连接机制
		 * @param keep_alive 是否自动保持连接
		 * @param time_interval 检测时间间隔
		 */
		bool TCPSocket::SetKeepAlive(bool keep_alive,const double time_interval)
		{
			int flag=(keep_alive?1:0);

			setsockopt(ThisSocket,SOL_SOCKET,SO_KEEPALIVE,&flag,sizeof(flag));

			timeval tv;

			SetTimeVal(tv,time_interval);

			setsockopt(ThisSocket,IPPROTO_TCP,TCP_KEEPIDLE,&tv,sizeof(timeval));
			setsockopt(ThisSocket,IPPROTO_TCP,TCP_KEEPINTVL,&tv,sizeof(timeval));
			setsockopt(ThisSocket,IPPROTO_TCP,TCP_KEEPCNT,&tv,sizeof(timeval));

			return(true);
		}

		/**
		* 使用指定socket
		* @param sock 指定socket编号
		* @param addr socket地址
		*/
		void TCPSocket::UseSocket(int sock,const sockaddr_in *addr)
		{
			if(sock==-1)return;

			ThisSocket=sock;

			if(addr)
				memcpy(&ThisAddr,addr,sizeof(sockaddr_in));
			else
			{
				socklen_t len=sizeof(sockaddr_in);

				getpeername(sock,(sockaddr *)&ThisAddr,&len);
			}

			FD_ZERO(&local_set);
			FD_SET(sock,&local_set);
		}

		/**
		 * 取得当前socket是否还在联接状态
		 */
		bool TCPSocket::IsConnect()
		{
			return(ThisSocket!=-1);
		}

		bool TCPSocket::ReConnect()
		{
			this->CloseSocket();

			ThisSocket=CreateTCPConnect(ThisAddr);

			return(ThisSocket>=0);
		}

		/**
		 * 等待数据接收
		 * @param wait_time 等待的时间
		 * @return >0 有数据
		 * @return 0 无数据，但不是错误（如超时）
		 * @return <0 出错
		 */
		int TCPSocket::WaitRecv(double wait_time)
		{
			if(ThisSocket==-1)return(-1);

			SetTimeVal(time_out,wait_time);

			memcpy(&recv_set,&local_set,sizeof(fd_set));
			memcpy(&err_set,&local_set,sizeof(fd_set));

			const int result=select(ThisSocket+1,&recv_set,nullptr,&err_set,&time_out);

			if(result<0)
			{
				int err=GetLastSocketError();

				return -err;
			}

			if(FD_ISSET(ThisSocket,&recv_set))
				return(1);

			if(FD_ISSET(ThisSocket,&err_set))
				return(-1);

			return(0);
		}
	}//namespace network
}//namespace hgl
