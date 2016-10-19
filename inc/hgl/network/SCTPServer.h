#ifndef HGL_NETWORK_SCTP_SERVER_INCLUDE
#define HGL_NETWORK_SCTP_SERVER_INCLUDE

#include<hgl/network/AcceptServer.h>
#include<hgl/network/SCTPSocket.h>
namespace hgl
{
	namespace network
	{
		/**
		 * SCTP一对一模式(one to one,tcp-style)服务器端
		 */
		class SCTPO2OServer:public AcceptServer
		{
			int CreateServerSocket() override final;												///<创建Server Socket
		};//class SCTPO2OServer

		/**
		 * SCTP一对多模式(one to many,udp-style)服务器端
		 */
		class SCTPO2MServer:public SCTPO2MSocket
		{
		public:

			virtual bool CreateServer(const sockaddr_in &,const uint ml=HGL_SERVER_LISTEN_COUNT);	///<创建服务器
					bool CreateServer(const char *,uint,const uint ml=HGL_SERVER_LISTEN_COUNT);		///<创建服务器
					bool CreateServer(uint port,const uint ml=HGL_SERVER_LISTEN_COUNT)				///<创建服务器
					{return CreateServer(nullptr,port,ml);}

			virtual void CloseServer();																///<关闭服务器

					/**
					* 设置是否使用堵塞方式传输
					* @param block 是否使用堵塞方式(true/false)
					* @param sto 发送数据超时时间(单位: 秒)
					* @param rto 接收数据超时时间(单位: 秒)
					*/
					void	SetBlock(bool block,double sto=HGL_NETWORK_TIME_OUT,
												double rto=HGL_NETWORK_TIME_OUT)					///<设置是否使用堵塞方式
					{
						SetSocketBlock(ThisSocket,block,sto,rto);
					}

					void	SetAutoClose(const int to);											///<设置自动关闭时间
		};//class SCTPO2MServer
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SCTP_SERVER_INCLUDE
