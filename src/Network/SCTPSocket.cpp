#include<hgl/network/SCTPSocket.h>

namespace hgl
{
	namespace network
	{
		SCTPSocket::SCTPSocket()
		{
			memset(&address,0,sizeof(sockaddr_in));
		}

		bool SCTPSocket::InitDataIOEvent()
		{
			struct sctp_event_subscribe events;

			hgl_zero(events);

			events.sctp_data_io_event=1;

			return(setsockopt(ThisSocket,SOL_SCTP,SCTP_EVENTS,(const void *)&events,sizeof(events))!=-1);
		}

		bool SCTPSocket::GetMsg()
		{
			sctp_initmsg msg;
			socklen_t len;

			len=sizeof(sctp_initmsg);

			if(getsockopt(ThisSocket,IPPROTO_SCTP,SCTP_INITMSG,(void *)&msg,&len)==-1)
				return(false);

			out_max_stream		=msg.sinit_num_ostreams;
			in_max_stream		=msg.sinit_max_instreams;
			init_max_attempts	=msg.sinit_max_attempts;
			init_max_init_timeo	=msg.sinit_max_init_timeo;

			return(true);
		}

		/**
		 * 初始化sctp socket
		 * @param this_socket socket号
		 * @param out_stream 允许发送的最大sctp流数(传入0表示不设置)
		 * @param in_stream 允许输入的最大sctp流数(-1表示同out_stream,0表示不设置)
		 * @param attempts 初始化重传多少次才认为不可达(0表示不设置)
		 * @param init_time_out init定时器超时时间单位为毫秒(0表示不设置)
		 * @return 是否成功
		 */
		bool SCTPSocket::InitMsg(int out_stream,int in_stream,int attempts,int init_time_out)
		{
			sctp_initmsg msg;

			if(in_stream==-1)
				in_stream=out_stream;

			msg.sinit_num_ostreams=out_stream;		//允许发送的最大sctp流数
			msg.sinit_max_instreams=in_stream;		//允许输入的最大sctp流数
			msg.sinit_max_attempts=attempts;		//重传多少次才认为不可达(0表示不设置)
			msg.sinit_max_init_timeo=init_time_out;	//重传超时时间，单位毫秒(0表示不设置)

			return(setsockopt(ThisSocket,IPPROTO_SCTP,SCTP_INITMSG,(void *)&msg,sizeof(sctp_initmsg))!=-1);
		}

		/**
		* 设置是否使用无延迟模式(注：无延迟模式也无缓冲，有利于小包高响应应用，大包或无高响应需要求应用不要设置)
		* @param no_delay 是否无延迟
		*/
		bool SCTPSocket::SetNodelay(bool no_delay)
		{
			int flag=(no_delay?1:0);

			return(setsockopt(ThisSocket,IPPROTO_SCTP,SCTP_NODELAY,(void *)&flag,sizeof(flag))!=-1);
		}

		bool SCTPSocket::SetMaxStream(int ms)
		{
			out_max_stream=ms;
			in_max_stream=ms;

			return InitMsg(ThisSocket,ms,ms);
		}

		void SCTPSocket::UseSocket(int s,const sockaddr_in *sa)
		{
			ThisSocket=s;

			InitDataIOEvent();
			GetMsg();

			if(sa)
				memcpy(&address,sa,sizeof(sockaddr_in));
			else
				memset(&address,0,sizeof(sockaddr_in));
		}

		/**
		 * 发送一个数据包
		 * @param buf 数据包内存指针
		 * @param len 数据包字节数
		 * @param stream 数据流编号
		 * @return 是否发送成功
		 */
		bool SCTPO2OSocket::SendMsg(const void *buf,int len,uint16 stream)
		{
			if(!buf||len<=0)
				return(false);

			if(stream>=out_max_stream)
				return(false);

			size_t result_size=sctp_sendmsg(ThisSocket,buf,len,
											nullptr,		//目标地址，一对一模式不需要设置
											0,				//上一个参数的长度
											0,				//ppid			净荷协议标识符
											0,				//flags
											stream,			//
											0,				//time to live	生命周期
											0);				//context 		上下文
			//两段代码效果一样
// 			hgl_zero(sri_send);
// 			sri_send.sinfo_stream=stream;
//
// 			size_t result_size=sctp_send(ThisSocket,buf,len,&sri_send,0);

			return(result_size==len);
		}

		/**
		 * 接收一个数据包
		 * @param stream 数据流编号
		 * @return 是否接收成功
		 */
		bool SCTPO2OSocket::RecvMsg(MemBlock<char> *mb,uint16 &stream)
		{
			if(!mb)return(false);

			int flags;
			int len;
			int off=0;

			hgl_zero(sri_recv);

			while(true)
			{
				len=sctp_recvmsg(ThisSocket,mb->GetData()+off,mb->GetMaxBytes()-off,
								nullptr,		//来源地址，一对一模式不需要
								0,				//上一个参数的长度
								&sri_recv,
								&flags);

				if(len<0)
					return(false);

				std::cout<<"sctp_recvmsg return "<<len<<", flags="<<flags
					<<", ssn:"<<sri_recv.sinfo_ssn
					<<", flags:"<<sri_recv.sinfo_flags
					<<", ppid:"<<sri_recv.sinfo_ppid
					<<", tsn:"<<sri_recv.sinfo_tsn
					<<", assoc_id:"<<sri_recv.sinfo_assoc_id
					<<std::endl;

				if(flags&MSG_EOR)
				{
					mb->SetLength(len);
					stream=sri_recv.sinfo_stream;
					return(true);
				}

				off+=len;

				mb->SetLength(mb->GetMaxLength()+1024);
			}
		}

		bool SCTPO2MSocket::Create()
		{
			int s=socket(AF_INET,SOCK_SEQPACKET,IPPROTO_SCTP);

			if(s<0)
				return(false);

			UseSocket(s);
			return(true);
		}

		bool SCTPO2MSocket::SendMsg(const sockaddr_in *sa,void *buf,int len,uint16 stream)
		{
			if(!sa||!buf||len<=0)
				return(false);

			const socklen_t sal=sizeof(sockaddr_in);

			size_t result_size=sctp_sendmsg(ThisSocket,buf,len,
											(sockaddr *)sa,	//目标地址
											sal,			//上一个参数的长度
											0,				//ppid
											0,				//flags
											stream,			//
											0,				//time to live
											0);				//context

			return(result_size==len);
		}

		bool SCTPO2MSocket::RecvMsg(sockaddr_in &sa,void *buf,int max_len,int &len,uint16 &stream)
		{
			struct sctp_sndrcvinfo sndrcvinfo;
			int flags;
			socklen_t sal=sizeof(sockaddr_in);

			hgl_zero(sndrcvinfo);

			len=sctp_recvmsg(ThisSocket,buf,max_len,
							(sockaddr *)&sa,		//来源地址
							&sal,					//上一个参数的长度
							&sndrcvinfo,
							&flags);

			if(len<0)
				return(false);

			stream=sndrcvinfo.sinfo_stream;
			return(true);
		}
	}//namespace network
}//namespace hgl
