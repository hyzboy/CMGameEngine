#ifndef HGL_NETWORK_SOCKET_MANAGE_THREAD_INCLUDE
#define HGL_NETWORK_SOCKET_MANAGE_THREAD_INCLUDE

#include<hgl/thread/Thread.h>
#include<hgl/thread/SemLock.h>
#include<hgl/type/Set.h>
#include<hgl/type/List.h>
#include<hgl/type/Map.h>
#include<hgl/network/SocketEvent.h>
#include<hgl/network/IOSocket.h>
namespace hgl
{
	namespace network
	{
		class Socket;
		class OnewaySocketManageBase;

		OnewaySocketManageBase *CreateRecvSocketManage(int max_user);										///<创建一个只处理读取的Socket管理器
		OnewaySocketManageBase *CreateSendSocketManage(int max_user);										///<创建一个只处理发送的Socket管理器

		/**
		 * Socket管理线程
		 */
		class SocketManageThread:public Thread
		{
		protected:

			OnewaySocketManageBase *sock_manage;													///<Socket管理器

			ThreadMutexObject<Map<int,IOSocket *>> sock_set;										///<Socket合集

			List<SocketEvent> event_list;															///<Socket事件列表
			List<SocketEvent> error_list;															///<Socket错误列表

			List<IOSocket *> error_socket_list;

			Set<int> delete_list;

			virtual void ProcSocketDelete();														///<处理要被删除的Socket

		protected:

			SemThreadMutex<Set<IOSocket *>> sock_join_set;											///<Socket加入合集

			virtual void ProcSocketJoin();															///<处理Socket加入

		public:

			double time_out;																		///<超时时间

		public:

			SocketManageThread();
			virtual ~SocketManageThread();

			virtual bool Execute()override;														///<本线程刷新函数

			virtual bool ProcStartThread()override=0;
			virtual void ProcError(IOSocket **,const int)=0;										///<处理错误Socket列表函数
			virtual bool ProcEvent(IOSocket *,int)=0;												///<处理事件Socket列表函数(需使用者自行重载处理)

		public:

			virtual bool Join(IOSocket *);															///<加入一个Socket
			virtual int Join(IOSocket **,const int);												///<加入一批Socket
			virtual bool Unjoin(IOSocket *);														///<分离一个Socket

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

			virtual bool ProcStartThread()
			{
				this->sock_manage=CreateRecvSocketManage(max_user);

				return(this->sock_manage);
			}

			virtual bool ProcEvent(IOSocket *sock,int size)								            ///<处理Socket接收事件函数
			{
				return(sock->ProcRecv(size)>=0);
			}

			virtual void ProcError(IOSocket **,const int)=0;							            ///<处理Socket出错事件函数
		};//class RecvSocketManageThread

		/**
		 * 发送数据Socket管理线程
		 */
		class SendSocketManageThread:public SocketManageThread
		{
			int max_user;

		public:

			SendSocketManageThread(int _max_user):SocketManageThread(),max_user(_max_user){}

			virtual bool ProcStartThread()
			{
				this->sock_manage=CreateSendSocketManage(max_user);

				return(this->sock_manage);
			}

			virtual bool ProcEvent(IOSocket *sock,int size)                                         ///<处理Socket发送事件函数
			{
				int left_bytes;

				return(sock->ProcSend(size,left_bytes)>=0);
			}

			virtual void ProcError(IOSocket **,const int)=0;							            ///<处理Socket出错事件函数
		};//class SendSocketManageThread
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SOCKET_MANAGE_THREAD_INCLUDE
