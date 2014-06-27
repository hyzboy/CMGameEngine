#ifndef HGL_NETWORK_SOCKER_MANAGE_BASE_INCLUDE
#define HGL_NETWORK_SOCKER_MANAGE_BASE_INCLUDE

#include<hgl/type/List.h>
#include<hgl/thread/Thread.h>
#include<hgl/network/IOSocket.h>
namespace hgl
{
	namespace network
	{
		class Socket;

		class SocketManageBase
		{
		public:

			virtual ~SocketManageBase()HGL_DEFAULT_MEMFUNC;

			virtual bool Join(IOSocket *)=0;
			virtual bool Unjoin(IOSocket *)=0;

			virtual bool Change(IOSocket *sock,bool r,bool w)=0;

			virtual void Clear()=0;

			virtual int Update(	List<SocketEvent> &recv_list,
								List<SocketEvent> &send_list,
								List<SocketEvent> &error_list,
								double time_out)=0;
		};//class SocketManageBase

		SocketManageBase *CreateSelectSocketManageBase(int max_connect);									///<创建一个Select Socket管理器
		SocketManageBase *CreateSocketManageBase(int max_connect);											///<创建一个Socket管理器

		/**
		 * Socket管理线程
		 */
		class SocketManageThread:public Thread
		{
		public:

			SocketManageThread(bool is_recv);
			virtual ~SocketManageThread()HGL_DEFAULT_MEMFUNC;

			virtual bool Join(IOSocket *)=0;
			virtual bool Unjoin(IOSocket *)=0;

			virtual void Clear()=0;

			virtual int	Update(List<SocketEvent> &event_list,
							   List<SocketEvent> &error_list,
								const double time_out)=0;
		};//class SocketManageThread
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SOCKER_MANAGE_BASE_INCLUDE
