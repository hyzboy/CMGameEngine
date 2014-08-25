#ifndef HGL_NETWORK_SOCKET_MANAGE_THREAD_INCLUDE
#define HGL_NETWORK_SOCKET_MANAGE_THREAD_INCLUDE

#include<hgl/thread/Thread.h>
#include<hgl/thread/SemLock.h>
#include<hgl/type/Set.h>
#include<hgl/type/List.h>
#include<hgl/type/Map.h>
#include<hgl/network/SocketEvent.h>
namespace hgl
{
	namespace network
	{
		class Socket;
		class SocketManageBase;

		SocketManageBase *CreateRecvSocketManage(int max_user);										///<创建一个只处理读取的Socket管理器
		SocketManageBase *CreateSendSocketManage(int max_user);										///<创建一个只处理发送的Socket管理器

		/**
		 * Socket管理线程
		 */
		class SocketManageThread:public Thread
		{
		protected:

			SocketManageBase *sock_manage;															///<Socket管理器

			ThreadMutexObject<Map<int,Socket *>> sock_set;											///<Socket合集

			List<SocketEvent> event_list;															///<Socket事件列表
			List<SocketEvent> error_list;															///<Socket错误列表

			Set<int> delete_list;

			virtual void ProcSocketDelete();														///<处理要被删除的Socket

		protected:

			SemThreadMutex<Set<Socket *>> sock_join_set;											///<Socket加入合集

			virtual void ProcSocketJoin();															///<处理Socket加入

		public:

			double time_out;																		///<超时时间

		public:

			SocketManageThread();
			virtual ~SocketManageThread();

			virtual bool Execute()HGL_OVERRIDE;														///<本线程刷新函数

			virtual bool ProcStartThread()=0;
			virtual void ProcError(Socket *){}														///<处理错误Socket列表函数
			virtual bool ProcEvent(Socket *)=0;														///<处理事件Socket列表函数(需使用者自行重载处理)

		public:

			virtual bool Join(Socket *);															///<加入一个Socket
			virtual int Join(Socket **,const int);													///<加入一批Socket
			virtual bool Unjoin(Socket *);															///<分离一个Socket

			virtual int GetCount()const;															///<取得Socket数量
			virtual void Clear();																	///<清除所有Socket
		};//class SocketManageThread

		/**
		 * 接收数据Socket管理线程
		 */
		class RecvSocketManageThread:public SocketManageThread
		{
			int max_user;

		public:

			RecvSocketManageThread(int _max_user):SocketManageThread(),max_user(_max_user){}

			virtual bool ProcStartThread()HGL_OVERRIDE
			{
				this->sock_manage=CreateRecvSocketManage(max_user);

				return(this->sock_manage);
			}

			virtual bool ProcEvent(Socket *)HGL_OVERRIDE=0;
		};//class RecvSocketManageThread

		/**
		 * 发送数据Socket管理线程
		 */
		class SendSocketManageThread:public SocketManageThread
		{
			int max_user;

		public:

			SendSocketManageThread(int _max_user):SocketManageThread(),max_user(_max_user){}

			virtual bool ProcStartThread()HGL_OVERRIDE
			{
				this->sock_manage=CreateSendSocketManage(max_user);

				return(this->sock_manage);
			}

			virtual bool ProcEvent(Socket *)HGL_OVERRIDE=0;
		};//class SendSocketManageThread
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SOCKET_MANAGE_THREAD_INCLUDE
