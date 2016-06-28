#ifndef HGL_NETWORK_ThisSocket_INCLUDE
#define HGL_NETWORK_ThisSocket_INCLUDE

#include<hgl/network/Socket.h>
namespace hgl
{
	namespace network
	{
		/**
		 * 服务器端Socket共用基类
		 */
		class ServerSocket																			///服务器端Socket共用基类
		{
		protected:

			int ThisSocket;
            IPAddress *server_address;

			virtual int CreateServerSocket()=0;														///<创建Server Socket

		public:

			ServerSocket();
			virtual ~ServerSocket();

            const   IPAddress *GetServerAddress()const{return server_address;}                      ///<取得服务器IP地址
                    IPAddress *CreateIPAddress()const
                                {return server_address?server_address->Create():nullptr;}           ///<创建一个空的IP地址

			virtual bool CreateServer(const IPAddress *,const uint ml=HGL_SERVER_LISTEN_COUNT,bool reuse=true);	         ///<创建服务器
			virtual void CloseServer();                                                                                  ///<关闭服务器

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

					bool SetIPv6Only(bool);															///<设置仅使用IPv6
		};//class ServerSocket
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_ThisSocket_INCLUDE
