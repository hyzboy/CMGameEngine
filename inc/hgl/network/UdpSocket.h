#ifndef HGL_UDPSOCKET_INCLUDE
#define HGL_UDPSOCKET_INCLUDE

#include<hgl/type/DataType.h>
#include<hgl/network/Socket.h>
namespace hgl
{
	namespace network
	{
		/**
		* 这个类提供使用UDP协议的通信，但它并不提供可靠数据传输的支持。
		*/
		class UDPSocket:public Socket                                                               ///UDP通信类
		{
            IPAddress *bind_addr;
			IPAddress *tar_addr;

		public:	//事件函数

			virtual int ProcRecv(int=-1){return -1;}
			virtual int ProcSend(int,int &left_bytes){return -1;}

		public:

			UDPSocket();                                                                            ///<本类构造函数
			~UDPSocket();                                                                           ///<本类析构函数

			bool Create(const IPAddress *);                                                         ///<创建一个udp,并绑定一个IP地址与指定端口
			bool Create(int family);                                                                ///<创建一个udp

			uint GetBindPort()const{return bind_addr->GetPort();}									///<取得绑定端口

			void SetSendAddr(const IPAddress *);                                                    ///<设定发送地址

			int SendPacket(const void *,int);                                                       ///<发送数据包
			int SendPacket(IPAddress *,const void *,int);                                     ///<向指定地址发送数据包
			int RecvPacket(void *,int,IPAddress *);                                                 ///<接收数据包
		};//class UDPSocket

		/**
		 * 使用回呼事件机制的UDPSocket
		 */
		class UDPSocketCB:public UDPSocket
		{
		public:	//事件函数

			DefEvent(void,	OnDisconnect,	(UDPSocket *));
			DefEvent(int,	OnRecv,			(UDPSocket *,int));
			DefEvent(int,	OnSend,			(UDPSocket *,int,int &));

			virtual void ClearEvent()
			{
				OnDisconnect=nullptr;
				OnRecv		=nullptr;
				OnSend		=nullptr;
			}

		public:

			UDPSocketCB(){ClearEvent();}															///<本类构造函数
			virtual ~UDPSocketCB()HGL_DEFAULT_MEMFUNC;

			virtual void ProcDisconnect()
			{
				SafeCallEvent(OnDisconnect,(this));
			}

			virtual int ProcRecv(int size)
			{
				if(OnRecv==nullptr)return(-1);

#ifdef HGL_VARIADIC_TEMPLATES
				return OnRecv(this,size);
#else
				return CallEvent(OnRecv,(this,size));
#endif//HGL_VARIADIC_TEMPLATES
			}

			virtual int ProcSend(int size,int &left_bytes)
			{
				if(OnSend==nullptr)return(-1);

#ifdef HGL_VARIADIC_TEMPLATES
				return OnSend(this,size,left_bytes);
#else
				return CallEvent(OnSend,(this,size,left_bytes));
#endif//HGL_VARIADIC_TEMPLATES
			}
		};//class UDPSocketCB
	}//namespace network

	using namespace network;
}//namespace hgl
#endif//HGL_UDPSOCKET_INCLUDE
