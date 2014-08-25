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
		/**
		 * 多线程Socket I/O管理基类
		 */
		class MTSocketManageBase
		{
			template<typename T> friend class SocketManageThreadPass;

			virtual void ProcError(SocketManageThread *,IOSocket **,const int)=0;					///<处理出错Socket列表

		public:

			virtual SocketManageThread *CreateRecvSocketManageThread()=0;
			virtual SocketManageThread *CreateSendSocketManageThread()=0;
		};//class MTSocketManageBase

		/**
		 * Socket管理线程错误回馈模板<br>
		 */
		template<typename T> class SocketManageThreadPass:public T
		{
			MTSocketManageBase *manage;

		public:

			SocketManageThreadPass(MTSocketManageBase *sm,int max_socket_count):T(max_socket_count)
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
		class MTSocketManage:public MTSocketManageBase,public Thread
		{
			int max_socket_count;																	///<最大socket数
			int thread_count;																		///<线程数量

			ThreadMutexObject<Set<IOSocket *>> sock_set;											///<Socket合集

			SocketManageThread **recv_manage;														///<接收管理器
			SocketManageThread **send_manage;														///<发送管理器

		protected:

			SemSwapSet<IOSocket *> error_sock_set;

		protected:

			virtual void ProcError(SocketManageThread *,IOSocket **,const int);						///<处理出错Socket列表

		public:

			MTSocketManage();
			virtual ~MTSocketManage();

			virtual bool Init(int _max_socket_count,int _thread_count);								///<初始化

			virtual bool Join(IOSocket *);															///<增加一个Socket
			virtual bool Unjoin(IOSocket *);														///<移除一个Socket

			virtual bool Start();																	///<开始运行线程管理器

			virtual bool Execute();																	///<线程刷新函数(主要处理错误socket删除)

		public:

			int					GetThreadCount()const{return thread_count;}							///<取得线程数量

			SocketManageThread *GetRecvThread(int index)const										///<取得指定接收线程
			{return (index<0||index>=thread_count)?nullptr:recv_manage[index];}

			SocketManageThread *GetSendThread(int index)const										///<取得指定发送线程
			{return (index<0||index>=thread_count)?nullptr:send_manage[index];}

		public:

			virtual SocketManageThread *CreateRecvSocketManageThread()								///<创建Socket收接管理线程
			{
				return(new SocketManageThreadPass<RecvSocketManageThread>(this,max_socket_count));
			}

			virtual SocketManageThread *CreateSendSocketManageThread()								///<创建Socket发送管理线程
			{
				return(new SocketManageThreadPass<SendSocketManageThread>(this,max_socket_count));
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
