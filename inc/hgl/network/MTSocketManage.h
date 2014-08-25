#ifndef HGL_NETWORK_MULTI_THREAD_SOCKET_MANAGE_INCLUDE
#define HGL_NETWORK_MULTI_THREAD_SOCKET_MANAGE_INCLUDE

#include<hgl/type/Set.h>
#include<hgl/thread/Thread.h>
#include<hgl/thread/SemLock.h>
#include<hgl/thread/SwapColl.h>
#include<hgl/network/SocketManageThread.h>
namespace hgl
{
	namespace network
	{
		class IOSocket;
		class SocketManageThread;

		/**
		 * 多线程Socket I/O管理
		 */
		class MTSocketManageBase:public Thread
		{
			template<typename T> friend class MTSocketWorkThread;

			virtual void ProcError(SocketManageThread *,IOSocket **,const int)=0;					///<处理出错Socket列表

		public:

			virtual SocketManageThread *CreateRecvSocketManageThread()=0;
			virtual SocketManageThread *CreateSendSocketManageThread()=0;
		};//class MTSocketManageBase

		template<typename T> class MTSocketWorkThread:public T
		{
			MTSocketManageBase *manage;

		public:

			MTSocketWorkThread(MTSocketManageBase *sm,int max_socket_count):T(max_socket_count)
			{
				manage=sm;
			}

			virtual void ProcError(IOSocket **sock_list,const int count)HGL_OVERRIDE
			{
				manage->ProcError(this,sock_list,count);			//将错误回传给MTSocketManage
			}
		};//class MTSocketRecvThread

		/**
		 * 多线程Socket I/O管理
		 */
		class MTSocketManage:public MTSocketManageBase
		{
			int max_socket_count;																	///<最大socket数
			int thread_count;																		///<线程数量

			ThreadMutexObject<Set<IOSocket *>> sock_set;											///<Socket合集

			SocketManageThread **recv_manage;														///<接收管理器
			SocketManageThread **send_manage;														///<发送管理器

		protected:

			SemSwapSet<IOSocket *> error_sock_set;
			Set<IOSocket *> proc_error_sock_set;

		protected:

			virtual void ProcError(SocketManageThread *,IOSocket **,const int);						///<处理出错Socket列表

		public:

			MTSocketManage();
			virtual ~MTSocketManage();

			bool Init(int _max_socket_count,int _thread_count);										///<初始化

			bool Join(IOSocket *);																	///<增加一个Socket
			bool Unjoin(IOSocket *);																///<移除一个Socket

			bool Execute();

		public:

			virtual SocketManageThread *CreateRecvSocketManageThread()
			{
				return(new MTSocketWorkThread<RecvSocketManageThread>(this,max_socket_count));
			}

			virtual SocketManageThread *CreateSendSocketManageThread()
			{
				return(new MTSocketWorkThread<SendSocketManageThread>(this,max_socket_count));
			}

			virtual void ProcError(IOSocket *sock)													///<处理错误Socket
			{
				if(!sock)return;

				delete sock;
			}
		};//class MTSocketManage
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_MULTI_THREAD_SOCKET_MANAGE_INCLUDE
