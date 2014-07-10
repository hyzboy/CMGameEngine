#include<hgl/network/TCPSocket.h>
#include<hgl/thread/RingBuffer.h>
#include<hgl/MemBlock.h>
#include<hgl/LogInfo.h>

namespace hgl
{
	namespace network
	{
		int TCPSocketRB::ProcRecv(int prev_recv_size,const double cur_time)
		{
			TCPSocket::ProcRecv(prev_recv_size,cur_time);

			const int recv_max=RecvBuffer->SafeWriteStart();

			if(recv_max<=0)
			{
				LOG_INFO(OS_TEXT("TCPSocketRB::ProcRecv() error,Socket:")+OSString(ThisSocket)+OS_TEXT(" recv_max=")+OSString(recv_max));
				return(recv_max);
			}

			recv_temp_buffer->SetLength(recv_max);

			char *p=recv_temp_buffer->data();
			int total=0;
			int recv_size;

			do
			{
				recv_size=recv(ThisSocket,p,recv_max-total,0);

				if(recv_size>0)
				{
					p+=recv_size;
					total+=recv_size;

					recv_total+=recv_size;
				}
				else
				{
					int err=GetLastSocketError();

					if(err==EAGAIN)			//结果是EAGAIN的话代表正常读完
						continue;

					total=-1;

					LOG_INFO(OS_TEXT("TCPSocketRB::ProcRecv() error,Socket:")+OSString(ThisSocket)+OS_TEXT(" recv result=")+OSString(recv_size)+OS_TEXT(",errno=")+OSString(err));

					//recv_size==0表示socket被关闭
				}
			}while(recv_size>0);

			if(total>0)
			{
// 			    LOG_INFO(OS_TEXT("TCPSocketRB ")+OSString(ThisSocket)+OS_TEXT(" recv ")+OSString(total)+OS_TEXT(" bytes data."));

				if(total>recv_max)		//接收的居然比处理的还快，要么处理的太慢，要么缓冲区太小
				{
					LOG_ERROR(OS_TEXT("TCPSocketRB::ProcRecv() recv buffer overflow! program too slow or recv buffer too small"));

					RecvBuffer->Write(recv_temp_buffer->data(),recv_max);		//写入可以写入的最大长度

					total=-1;
				}
				else
					RecvBuffer->Write(recv_temp_buffer->data(),total);
			}

			RecvBuffer->SafeWriteEnd();
			return(total);
		}

		int TCPSocketRB::ProcSend(int,int &left_bytes)
		{
			int send_max=SendBuffer->SafeReadStart();

			if(send_max<=0)
			{
				LOG_INFO(OS_TEXT("TCPSocketRB::ProcSend() error,Socket:")+OSString(ThisSocket)+OS_TEXT(" send_max=")+OSString(send_max));
				left_bytes=0;
				return(send_max);
			}

			send_max=SendBuffer->Peek(send_temp_buffer,send_max);

			if(send_max<=0)
			{
				LOG_INFO(OS_TEXT("TCPSocketRB::ProcSend() SendBuffer->Peek error,Socket:")+OSString(ThisSocket)+OS_TEXT(" send_max=")+OSString(send_max));

				left_bytes=0;
				SendBuffer->SafeReadEnd();
				return send_max;
			}

			int total=0;
			int send_size;
			const char *p=send_temp_buffer;

			do
			{
//				LOG_INFO(OS_TEXT("socket ")+OSString(ThisSocket)+OS_TEXT(" send_max=")+OSString(send_max)+OS_TEXT(" total=")+OSString(total));
				send_size=send(ThisSocket,p,send_max-total,0);

				if(send_size>0)
				{
					p+=send_size;
					total+=send_size;

					send_total+=send_size;
				}
				else
				{
					int err=GetLastSocketError();

					if(err==EAGAIN)
						continue;

					LOG_INFO(OS_TEXT("TCPSocketRB::ProcSend() error,Socket:")+OSString(ThisSocket)+OS_TEXT(" send result=")+OSString(send_size)+OS_TEXT(",errno=")+OSString(err));

					total=-1;
					break;
				}
			}while(send_size>0&&total<send_max);

			if(total>0)
			{
//			    LOG_INFO(OS_TEXT("TCPSocketRB ")+OSString(ThisSocket)+OS_TEXT(" send ")+OSString(total)+OS_TEXT(" bytes data. left ")+OSString(send_max-total)+OS_TEXT(" bytes data"));

				SendBuffer->Skip(total);
			}

			left_bytes=SendBuffer->GetReadSize();		//剩下的可读数据长度
			left_send_bytes=left_bytes;					//更新到供外面读取的left_send_bytes

			SendBuffer->SafeReadEnd();
			return(total);
		}
	}//namespace network
}//namespace hgl
