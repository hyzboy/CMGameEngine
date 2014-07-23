#ifndef HGL_NETWORK_SOCKET_MANAGE_INCLUDE
#define HGL_NETWORK_SOCKET_MANAGE_INCLUDE

#include<hgl/type/Map.h>
#include<hgl/type/Set.h>
#include<hgl/network/IOSocket.h>
#include<hgl/thread/ThreadMutex.h>
#include<hgl/MemBlock.h>
#include<hgl/network/SocketEvent.h>
namespace hgl
{
	namespace network
	{
		class SocketManageBase;

		/**
		 * Socket管理类
		 */
		class SocketManage																			///Socket管理类
		{
			struct SocketItem
			{
				IOSocket *sock;
				bool r;			//读监听
				bool w;			//写监听

			public:

				SocketItem()
				{
					sock=nullptr;
					r=false;
					w=false;
				}

				SocketItem(IOSocket *s)
				{
					sock=s;
					r=true;
					w=true;
				}

				bool operator == (const SocketItem &si)const
				{
					if(si.sock!=sock)
						return(false);

					if(si.r!=r)
						return(false);

					if(si.w!=w)
						return(false);

					return(true);
				}
			};//struct SocketItem

			typedef Map<int,SocketItem> SocketList;

		protected:

			SocketManageBase *manage;																///<具体管理实现

			int max_connect;																		///<最大连接数

			atom_int cur_count;																		///<当前连接数

			double time_out;																		///<超时时间

			ThreadMutex lock;																		///<安全锁

			SocketList sock_list;																	///<所有socket列表

			List<SocketEvent>	recv_event_list;													///<接收Socket列表
			List<SocketEvent>	send_event_list;													///<发送Socket列表
			List<SocketEvent>	error_event_list;													///<出错Socket列表

			List<IOSocketClassEvent>	recv_list;													///<接收Socket列表
			List<IOSocketClassEvent>	send_list;													///<发送Socket列表

			Set<IOSocket *>				error_set;													///<错误Socket合集

// 			Set<IOSocket *>				no_send_set;												///<无发送数据Socket合集
// 			ThreadMutex					send_set_lock;												///<有无发送数据Socket合集锁

			void ConvEventList(List<IOSocketClassEvent> &,const List<SocketEvent> &);

		protected:

			virtual bool _Join(IOSocket *);
			virtual int _Unjoin(IOSocket **,const int);
			virtual bool _Unjoin(IOSocket *);
			virtual void _Clear();

		public:

			SocketManage(int/*max_connect*/);
			virtual ~SocketManage();

			SocketManageBase *			GetRecvManage	(){return manage;}						///<返回接收处理

							double		GetTimeOut		()const{return time_out;}					///<取得当前超时设置
			virtual 		void		SetTimeOut		(double);									///<设置新的超时时间

					const	int			GetCount		()const{return cur_count;}					///<取得加入的Socket数量

			virtual 		bool		Join			(IOSocket *);								///<加入一个Socket连接
			virtual 		bool		Unjoin			(IOSocket *);								///<分离一个Socket连接
			virtual			int			Unjoin			(IOSocket **,const int);					///<分离一批Socket连接

			virtual 		void		Clear			();											///<分离所有连接

							bool		Update			(const double );

		public:

			const List<IOSocketClassEvent> &GetRecvList(){return recv_list;}						///<取得有接收数据的连接列表
			const List<IOSocketClassEvent> &GetSendList(){return send_list;}
			const Set<IOSocket *> &			GetErrorSet(){return error_set;}						///<取得出错的连接列表
		};//class SocketManage
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SOCKET_MANAGE_INCLUDE
