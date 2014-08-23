#ifndef HGL_NETWORK_MULTI_THREAD_SOCKET_MANAGE_INCLUDE
#define HGL_NETWORK_MULTI_THREAD_SOCKET_MANAGE_INCLUDE

#include<hgl/network/SocketManageThread.h>
namespace hgl
{
	namespace network
	{
		class IOSocket;
		class IOSocketManage;

		class SocketManageThread;

		SocketManageThread *CreateRecvSocketManageThread(int);
		SocketManageThread *CreateSendSocketManageThread(int);

		/**
		 * 多线程Socket I/O管理
		 */
		class MTSocketManage
		{
			int max_socket_count;																	///<最大socket数
			int thread_count;																		///<线程数量

			SocketManageThread **recv_manage;														///<接收管理器
			SocketManageThread **send_manage;														///<发送管理器

		public:

			MTSocketManage()
			{
				max_socket_count=0;
				thread_count=0;

				recv_manage=nullptr;
				send_manage=nullptr;
			}

			~MTSocketManage()
			{
				SAFE_CLEAR_OBJECT_ARRAY(recv_manage,thread_count);
				SAFE_CLEAR_OBJECT_ARRAY(send_manage,thread_count);
			}

			/**
			 * 初始化多线程Socket I/O管理器
			 * @param msc 最大socket数量
			 * @param tc 线程数
			 * @return 是否初始化成功
			 */
			bool Init(int msc,int tc)																///<初始化
			{
				if(msc<=0)return(false);
				if(tc<=0)return(false);

				max_socket_count=msc;
				thread_count=tc;

				recv_manage=new SocketManageThread *[thread_count];
				send_manage=new SocketManageThread *[thread_count];

				for(int i=0;i<thread_count;i++)
				{
					recv_manage[i]=CreateRecvSocketManageThread();
					send_manage[i]=CreateSendSocketManageThread();

					recv_manage[i]->Start();
					send_manage[i]->Start();
				}

				return(true);
			}

			bool Join(IOSocket *s)																	///<增加一个Socket
			{
				if(!s)return(false);


			}

			bool Unjoin(IOSocket *s)																///<移除一个Socket
			{
				if(!s)return(false);

			}
		};//class MTSocketManage
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_MULTI_THREAD_SOCKET_MANAGE_INCLUDE
