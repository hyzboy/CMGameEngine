#include<hgl/network/SCTPClient.h>
#include<hgl/LogInfo.h>
#include<netinet/sctp.h>

namespace hgl
{
	namespace network
	{
		SCTPO2OClient::SCTPO2OClient()
		{
		}

		SCTPO2OClient::SCTPO2OClient(int sock)
		{
			UseSocket(sock);
		}

		SCTPO2OClient::~SCTPO2OClient()
		{
		}

		/**
		 * 连接到服务器
		 * @param addr 服务器地址
		 * @return 是否连接成功
		 */
		bool SCTPO2OClient::Connect(const sockaddr_in &addr)
		{
			CloseSocket();

			if(!CreateSocket(AF_INET,SOCK_STREAM,IPPROTO_SCTP))
				return(false);

			if(connect(ThisSocket,(sockaddr *)&addr,sizeof(addr)))
			{
				os_char ipstr[32];

				SockToStr(addr,ipstr,true);

				LOG_HINT(OS_TEXT("Don't Connect to SCTPServer ")+OSString(ipstr));
				CloseSocket();
				return(false);
			}

			//			LOG_INFO(u"connect "+u8_to_u16(host)+u':'+UTF16String(port)+u" ok,socket:"+UTF16String(ThisSocket));

			InitDataIOEvent();
			GetMsg();

			//SetBlock(true,TimeOut);	//阻塞模式

			return(true);
		}

		/**
		 * 连接到服务器
		 * @param host 服务器地址
		 * @param port 端口
		 * @return 是否连接成功
		 */
		bool SCTPO2OClient::Connect(const char *host,int port)
		{
			sockaddr_in addr;

			if(FillAddr(&addr,host,port)==false)
			{
				LOG_HINT(OS_TEXT("SCTP connect error in ")+OSString(host)+OS_TEXT(":")+OSString(port));
				CloseSocket();
				return(false);
			}

			return Connect(addr);
		}
	}//namespace network
}//namespace hgl
