#ifndef HGL_NETWORK_SOCKET_MANAGE_THREAD_INCLUDE
#define HGL_NETWORK_SOCKET_MANAGE_THREAD_INCLUDE

#include<hgl/thread/Thread.h>
#include<hgl/thread/ThreadMutex.h>
#include<hgl/type/Set.h>
#include<hgl/type/List.h>
#include<hgl/network/SocketEvent.h>
namespace hgl
{
	namespace network
	{
		class Socket;
		class SocketManageBase;

		/**
		 * Socket管理线程
		 */
		class SocketManageThread:public Thread
		{
		protected:

			SocketManageBase *sock_manage;															///<Socket管理器

			Set<Socket *> sock_set;																	///<Socket合集

			List<SocketEvent> event_list;															///<Socket事件列表
			List<SocketEvent> error_list;															///<Socket错误列表

		protected:

			Set<Socket *> sock_join_set;															///<Socket加入合集
			ThreadMutex sock_join_lock;																///<Socket加入合集锁

			virtual void ProcSocketJoin();															///<处理Socket加入

		public:

			double time_out;																		///<超时时间

		public:

			SocketManageThread(SocketManageBase *);
			virtual ~SocketManageThread();

			virtual bool Execute()HGL_OVERRIDE;														///<本线程刷新函数

		public:

			virtual bool Join(Socket *);															///<加入一个Socket
			virtual int Join(Socket **,int);														///<加入一批Socket
			virtual bool Unjoin(Socket *);															///<分离一个Socket

			virtual int GetCount()const;															///<取得Socket数量
			virtual void Clear();																	///<清除所有Socket
		};//class SocketManageThread
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SOCKET_MANAGE_THREAD_INCLUDE
