#ifndef HGL_NETWORK_TCP_SOCKET_RB_THREAD_INCLUDE
#define HGL_NETWORK_TCP_SOCKET_RB_THREAD_INCLUDE

#include<hgl/type/Set.h>
#include<hgl/type/List.h>
#include<hgl/thread/Thread.h>
#include<hgl/thread/ThreadMutex.h>
#include<hgl/thread/RWLock.h>

namespace hgl
{
	class Semaphore;

	namespace network
	{
		class SocketManageCB;
		class TCPSocketRB;

		/**
		* TCPSocketRB数据收发管理线程
		*/
		class TCPSocketRBThread:public Thread																///TCPSocketRB数据收发管理线程
		{
		protected:

			RWLock sock_lock;

			SocketManageCB *sock_manage;

			Set<TCPSocketRB *> sock_set;

		protected:

			ThreadMutexObject<List<TCPSocketRB *> > join_list;
			Semaphore *join_sem;

			virtual void ProcJoin();

		protected:

			ThreadMutexObject<Set<TCPSocketRB *> > unjoin_set;

			virtual void ProcUnjoin();

		public:

			TCPSocketRBThread(int _max_user=1024,const double _time_out=0.25f);
			virtual ~TCPSocketRBThread();

			virtual bool Execute();

		public:

			virtual bool Join(TCPSocketRB **,int);															///<加入一批连接
			virtual bool Unjoin(TCPSocketRB **,int);														///<断开一批连接
		};//class TCPSocketRBThread
	}//namespace network
}//hgl
#endif//HGL_NETWORK_TCP_SOCKET_RB_THREAD_INCLUDE
