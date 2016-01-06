#ifndef HGL_NETWORK_SOCKET_THREAD_SERVER_INCLUDE
#define HGL_NETWORK_SOCKET_THREAD_SERVER_INCLUDE

#include<hgl/thread/Thread.h>
#include<hgl/Other.h>
#include<hgl/network/Socket.h>
namespace hgl
{
	namespace network
	{
		/**
		* Socket线程独立服务器模块
		*/
		template<typename FO,typename S,typename O> class SocketThreadServer:public FO
		{
			S *server;

		public:

			SocketThreadServer(const IPAddress *addr)
			{
                if(!addr)
                {
                    server=nullptr;
                    LOG_ERROR(OS_TEXT("Create SocketThreadServer failed! no address"));
                    this->fos=fosExitGame;
                    return;
                }

				server	=new S();

				if(!server->CreateServer(addr))
				{
					LOG_ERROR(OS_TEXT("Create SocketThreadServer failed!"));
					this->fos=fosExitGame;
					return;
				}
			}

			~SocketThreadServer()
			{
				delete server;
			}

			S *GetServer()
			{
				return server;
			}

			void Update()
			{
				IPAddress *sa=server->CreateIPAddress();

				const int sock=server->Accept(sa);							//接入一个连接

				if(sock<0)		//出错
				{
                    delete sa;

                    this->fos=fosExitGame;

					LOG_INFO(OS_TEXT("SocketThreadrServer Accept error!"));
				}
				else
				if(sock>0)
				{
					O *obj=new O(sock,sa);

					if(!obj)
					{
						CloseSocket(sock);
						WaitTime(1);
						return;
					}

					obj->Thread::Start();
				}
			}
		};//class template<typename S,typename O> class SocketThreadServer
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SOCKET_THREAD_SERVER_INCLUDE
