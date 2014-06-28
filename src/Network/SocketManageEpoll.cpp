#include<hgl/network/SocketManageBase.h>
#include<hgl/LogInfo.h>

#include<unistd.h>
#include<sys/epoll.h>

namespace hgl
{
	namespace network
	{
		namespace
		{
			class SocketManageBaseEpoll:public SocketManageBase
			{
				int					epoll_fd;
				epoll_event *		event_list;

				uint				event;

				int					max_connect;
				int					cur_count;

			private:

				bool epoll_add(int sock)
				{
					epoll_event ev;

					memset(&ev,0,sizeof(epoll_event));

					ev.data.fd=sock;
					ev.events=event|EPOLLET|EPOLLERR|EPOLLRDHUP|EPOLLHUP;

					return(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,sock,&ev)==0);
				}

				bool epoll_del(int sock)
				{
					//在内核版本 2.6.9 之前，EPOLL_CTL_DEL 要求一个 event 是非空的指针，尽管这个参数会被忽略。
					//从 Linux 2.6.9 开始，在执行 EPOLL_CTL_DEL 时 event 可以指定为 NULL。
					//我们不支持Linux 2.6.9之前的版本，所以直接传NULL
					return(epoll_ctl(epoll_fd,EPOLL_CTL_DEL,sock,nullptr)==0);
				}

			public:

				SocketManageBaseEpoll(int mc,int epfd,uint ev)
				{
					cur_count=0;
					max_connect=mc;

					epoll_fd=epfd;

					event_list=new epoll_event[max_connect];

					event=ev;
				}

				~SocketManageBaseEpoll()
				{
					delete[] event_list;

					close(epoll_fd);
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

				int Update(List<SocketEvent> &sock_list,List<SocketEvent> &error_list,double time_out)
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

					sock_list.SetCount(num);
					error_list.SetCount(num);

					epoll_event *ee=event_list;

					SocketEvent *sp=sock_list.GetData();
					SocketEvent *ep=error_list.GetData();

					int sock_num=0;
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
						}
						else
						if(ee->events&event)				//可以读数据
						{
							sp->sock=ee->data.fd;
							sp->size=-1;
							++sp;
							++sock_num;
						}

						++ee;
					}

					sock_list.SetCount(sock_num);
					error_list.SetCount(error_num);

					return(num);
				}
			};//class SocketManageBaseEpoll

			SocketManageBase *CreateSocketManageBaseEpoll(int max_user,uint event)
			{
				int epoll_fd=epoll_create(max_user);

				if(epoll_fd<0)
				{
					std::cout<<"epoll_create return error.cur epoll_count is "<<epoll_count<<" ,errno is "<<errno<<std::endl;
					return(nullptr);
				}

				return(new SocketManageBaseEpoll(max_user,epoll_fd,event));
			}
		}//namespace

		SocketManageBase *CreateRecvSocketManage(int max_user)
		{
			return CreateSocketManageBaseEpoll(max_user,EPOLLIN);
		}

		SocketManageBase *CreateSendSocketManage(int max_user)
		{
			return CreateSocketManageBaseEpoll(max_user,EPOLLOUT);
		}
	}//namespace network
}//namespace hgl
