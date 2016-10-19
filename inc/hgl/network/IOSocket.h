#ifndef HGL_NETWORK_IOSOCKET_INCLUDE
#define HGL_NETWORK_IOSOCKET_INCLUDE

#include<hgl/network/Socket.h>

namespace hgl
{
	namespace network
	{
		/**
		 * 输入输出用Socket
		 */
		class IOSocket:public Socket																///传输数据用Socket基类
		{
		protected:

			int64 send_total;																		///<发送字节数统计
			int64 recv_total;																		///<接收字节数统计

			double recv_time_out;																	///<数据接收超时

			double last_recv_time;																	///<最后一次接收数据的时间

			atom_int left_send_bytes;																///<剩余要发送的字节数

		public:

			IOSocket()
			{
				Clear();
			}

			virtual ~IOSocket()=default;

			virtual void	Clear()
			{
				send_total=0;
				recv_total=0;
				recv_time_out=HGL_NETWORK_DOUBLE_TIME_OUT;
				last_recv_time=0;
				left_send_bytes=0;
			}

			virtual void	CloseSocket()override													///<关闭连接
			{
				Socket::CloseSocket();
				Clear();
			}

			virtual const	double &GetRecvTimeOut()const{return recv_time_out;}					///<取得接收数据超时时间
			virtual 		void	SetRecvTimeOut(const double to){recv_time_out=to;}				///<设置接收数据超时时间

		public: //被动事件函数

			virtual int		ProcRecv(int recv_buf_size=-1,const double ct=0)						///<接收数据事件处理函数
			{
				last_recv_time=ct;
				return 0;
			}

			virtual int		ProcSend(int send_buf_size,int &left_bytes)								///<发送数据事件处理函数
			{
				left_bytes=left_send_bytes;
				return 0;
			}

		public:	//属性函数

					const	int64	GetSendTotal()const{return send_total;}							///<取得发送字节数统计
					const	int64	GetRecvTotal()const{return recv_total;}							///<取得接收字节数统计

					const	void	RestartLastRecvTime(){last_recv_time=0;}						///<复位最后获取数据时间
					const	double	GetLastRecvTime()const{return last_recv_time;}					///<取得最后获取数据时间
					const	bool	CheckRecvTimeOut(const double ct)								///<检测是否超时
					{
						if((last_recv_time>0)
						 &&(last_recv_time+recv_time_out<ct))
							return(true);

						return(false);
					}

			virtual 		void	IncSendBytes(const uint bytes){left_send_bytes+=bytes;}			///<增加发送出去的字节数
			virtual const	int		GetLeftSendBytes()const{return 0;}								///<取得剩余未发送字节数
		};//class IOSocket
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_IOSOCKET_INCLUDE
