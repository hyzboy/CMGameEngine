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
			virtual ~UDPSocket();                                                                   ///<本类析构函数

			virtual bool Create(const IPAddress *);                                                 ///<创建一个udp,并绑定一个IP地址与指定端口
			virtual bool Create(int family);                                                        ///<创建一个udp

                    uint GetBindPort()const{return bind_addr->GetPort();}							///<取得绑定端口

                    bool SetSendAddr(const IPAddress *);                                            ///<设定发送地址

                    int SendPacket(const void *,int);                                               ///<发送数据包
                    int SendPacket(IPAddress *,const void *,int);                                   ///<向指定地址发送数据包
                    int RecvPacket(void *,int,IPAddress *);                                         ///<接收数据包
		};//class UDPSocket

		/**
         * UDPLite协议封装使用
         */
		class UDPLiteSocket:public UDPSocket
		{
        public:

            virtual ~UDPLiteSocket()HGL_DEFAULT_MEMFUNC;

            bool Create(const IPAddress *)HGL_OVERRIDE;                                              ///<创建一个udp lite,并绑定一个IP地址与指定端口
			bool Create(int family)HGL_OVERRIDE;                                                     ///<创建一个udp lite

			void SetChecksumCoverage(int send_val=20,int recv_val=20);                               ///<设定UDPLite检验位长度,最小20
        };//class UDPLiteSocket

		/**
		 * 使用回呼事件机制的UDPSocket
		 */
        template<typename BASE> class _UDPSocketCB:public BASE
		{
		public:	//事件函数

			DefEvent(void,	OnDisconnect,	(BASE *));
			DefEvent(int,	OnRecv,			(BASE *,int));
			DefEvent(int,	OnSend,			(BASE *,int,int &));

			virtual void ClearEvent()
			{
				OnDisconnect=nullptr;
				OnRecv		=nullptr;
				OnSend		=nullptr;
			}

		public:

			_UDPSocketCB(){ClearEvent();}															///<本类构造函数
			virtual ~_UDPSocketCB()HGL_DEFAULT_MEMFUNC;

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
		};//class _UDPSocketCB

		using UDPSocketCB     =_UDPSocketCB<UDPSocket>;
        using UDPLiteSocketCB =_UDPSocketCB<UDPLiteSocket>;
	}//namespace network

	using namespace network;
}//namespace hgl
#endif//HGL_UDPSOCKET_INCLUDE
