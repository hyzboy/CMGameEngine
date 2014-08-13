#include"SocketManageBase.h"
#include<hgl/network/IOSocket.h>
#include<hgl/LogInfo.h>

#include<unistd.h>
#include<sys/epoll.h>
#include<iostream>

namespace hgl
{
	namespace network
	{
		static atom_int epoll_count(0);

		class SocketManageEpoll:public SocketManageBase
		{
			int					epoll_fd;
			epoll_event *		event_list;

			int					max_connect;
			int					cur_count;

		private:

			bool epoll_add(int sock)
			{
				epoll_event ev;

				memset(&ev,0,sizeof(epoll_event));

				ev.data.fd=sock;
				ev.events=EPOLLIN|EPOLLOUT|EPOLLET|EPOLLERR|EPOLLRDHUP|EPOLLHUP;

				return(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,sock,&ev)==0);
			}

			bool epoll_del(int sock)
			{
				//在内核版本 2.6.9 之前，EPOLL_CTL_DEL 要求一个 event 是非空的指针，尽管这个参数会被忽略。
				//从 Linux 2.6.9 开始，在执行 EPOLL_CTL_DEL 时 event 可以指定为 NULL。
				//我们不支持Linux 2.6.9之前的版本，所以直接传NULL
				return(epoll_ctl(epoll_fd,EPOLL_CTL_DEL,sock,nullptr)==0);
			}

			bool epoll_mod(int sock,bool r,bool w)
			{
				epoll_event ev;

				memset(&ev,0,sizeof(epoll_event));

				ev.data.fd=sock;
				ev.events=EPOLLET|EPOLLERR|EPOLLRDHUP|EPOLLHUP;

				if(r)ev.events|=EPOLLIN;
				if(w)ev.events|=EPOLLOUT;

				return(epoll_ctl(epoll_fd,EPOLL_CTL_MOD,sock,&ev)==0);
			}

		public:

			SocketManageEpoll(int mc,int epfd)
			{
				cur_count=0;
				max_connect=mc;

				epoll_fd=epfd;

				event_list=new epoll_event[max_connect];
			}

			~SocketManageEpoll()
			{
				delete[] event_list;

				close(epoll_fd);

				epoll_count--;
			}

			bool Join(IOSocket *sock)
			{
				epoll_add(sock->ThisSocket);

				sock->SetBlock(false);

				++cur_count;

				return(true);
			}

			bool Unjoin(IOSocket *sock)
			{
				if(epoll_fd==-1)
				{
					LOG_ERROR(OS_TEXT("SocketManageEpoll::Unjoin() epoll_fd==-1)"));
					return(false);
				}

				--cur_count;
				epoll_del(sock->ThisSocket);

				LOG_INFO(OS_TEXT("SocketManageEpoll::Unjoin() IOSocket:")+OSString(sock->ThisSocket));

				return(true);
			}

			bool Change(IOSocket *sock,bool r,bool w)
			{
				if(epoll_fd==-1)
				{
					LOG_ERROR(OS_TEXT("SocketManageEpoll::Unjoin() epoll_fd==-1)"));
					return(false);
				}

				return epoll_mod(sock->ThisSocket,r,w);
			}

			void Clear()
			{
				if(epoll_fd!=-1)
				{
					close(epoll_fd);
					epoll_fd=-1;
				}

				cur_count=0;
			}

			int Update(List<SocketEvent> &recv_list,List<SocketEvent> &send_list,List<SocketEvent> &error_list,double time_out)
			{
				if(epoll_fd==-1)
					return(-1);

				if(cur_count<=0)
					return(0);

				const int num=epoll_wait(epoll_fd,event_list,cur_count,time_out<0?-1:time_out*HGL_MILLI_SEC_PRE_SEC);

				if(num==0)
					return(0);

				if(num<0)
				{
					LOG_INFO(OS_TEXT("epoll return -1,errno: ")+OSString(errno));

					if(errno==EBADF
					 ||errno==EFAULT
					 ||errno==EINVAL)
						return(-1);

					return(0);
				}
// #ifdef _DEBUG
// 				else
// 				{
// 					OSString str(OS_TEXT("epoll["));
//
// 					str+=OSString(epoll_fd);
// 					str+=OS_TEXT("]:");
// 					str+=OSString(event_list[0].data.fd);
//
// 					for(int i=1;i<num;i++)
// 					{
// 						str+=',';
// 						str+=OSString(event_list[i].data.fd);
// 					}
//
// 					LOG_INFO(str);
// 				}
// #endif//_DEBUG

				recv_list.SetCount(num);
				send_list.SetCount(num);
				error_list.SetCount(num);

				epoll_event *ee=event_list;

				SocketEvent *rp=recv_list.GetData();
				SocketEvent *sp=send_list.GetData();
				SocketEvent *ep=error_list.GetData();
				int recv_num=0;
				int send_num=0;
				int error_num=0;

				for(int i=0;i<num;i++)
				{
					if(ee->events&(	EPOLLERR|			//出错了
									EPOLLRDHUP|			//对方关了
									EPOLLHUP))			//我方强制关了
					{
						LOG_ERROR("SocketManageEpoll Error,socket:"+OSString(ee->data.fd)+",epoll event:"+OSString(ee->events));

						ep->sock=ee->data.fd;
						ep->size=-1;
						++ep;
						++error_num;

						++ee;
						continue;
					}

					if(ee->events&EPOLLIN)				//可以读数据
					{
						rp->sock=ee->data.fd;
						rp->size=-1;
						++rp;
						++recv_num;
					}

					if(ee->events&EPOLLOUT)				//可以发数据
					{
						sp->sock=ee->data.fd;
						sp->size=-1;
						++sp;
						++send_num;
					}

					++ee;
				}

				recv_list.SetCount(recv_num);
				send_list.SetCount(send_num);
				error_list.SetCount(error_num);

				return(num);
			}
		};//class SocketManageEpoll

		SocketManageBase *CreateSocketManageBase(int max_user)
		{
			int epoll_fd=epoll_create(max_user);

			if(epoll_fd<0)
			{
				std::cout<<"epoll_create return error.cur epoll_count is "<<epoll_count<<" ,errno is "<<errno<<std::endl;
				return(nullptr);
			}

			epoll_count++;

			return(new SocketManageEpoll(max_user,epoll_fd));
		}
	}//namespace network
}//namespace hgl
