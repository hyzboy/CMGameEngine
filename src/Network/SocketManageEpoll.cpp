#include<hgl/network/SocketManageBase.h>
#include<hgl/network/Socket.h>
#include<hgl/LogInfo.h>

#include<unistd.h>
#include<sys/epoll.h>

namespace hgl
{
	namespace network
	{
		namespace
		{
            template<typename SMB,uint EVENT> class SocketManageEpoll:public SMB
			{
            private:

				int					epoll_fd;

				int					max_connect;
				int					cur_count;

            protected:

                epoll_event *       event_list;

			private:

				bool epoll_add(int sock)
				{
					epoll_event ev;

					memset(&ev,0,sizeof(epoll_event));

					ev.data.fd=	sock;
					ev.events=	EVENT       //要处理的事件
								|EPOLLET	//边缘模式(即读/写时，需要一直读/写直到出错为止；相对LT模式是只要有数据就会一直通知)
								|EPOLLERR	//出错
								|EPOLLRDHUP	//对方挂断
								|EPOLLHUP;	//挂断

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

				SocketManageEpoll(int mc,int epfd)
				{
					cur_count=0;
					max_connect=mc;

					epoll_fd=epfd;

					event_list=new epoll_event[max_connect];
				}

				SocketManageEpoll()
				{
					delete[] event_list;

					close(epoll_fd);
				}

				bool Join(int sock)
				{
					epoll_add(sock);

					SetSocketBlock(sock,false);

					++cur_count;

					return(true);
				}

				bool Join(const int *sock_list,int count)
                {
                    if(count<=0)return(false);

                    for(int i=0;i<count;i++)
                    {
                        epoll_add(*sock_list);

                        SetSocketBlock(*sock_list,false);

                        ++sock_list;
                    }

                    cur_count+=count;

                    return(true);
                }

				bool Unjoin(int sock)
				{
					if(epoll_fd==-1)
					{
						LOG_ERROR(OS_TEXT("SocketManageEpoll::Unjoin() epoll_fd==-1)"));
						return(false);
					}

					--cur_count;
					epoll_del(sock);

					LOG_INFO(OS_TEXT("SocketManageEpoll::Unjoin() Socket:")+OSString(sock));

					return(true);
				}

				bool Unjoin(const int *sock_list,int count)
				{
					if(epoll_fd==-1)
					{
						LOG_ERROR(OS_TEXT("SocketManageEpoll::Unjoin() epoll_fd==-1)"));
						return(false);
					}

					if(!sock_list||count<=0)return(false);

					for(int i=0;i<count;i++)
					{
						epoll_del(*sock_list);
						++sock_list;
					}

					cur_count-=count;
					LOG_INFO(OS_TEXT("SocketManageEpoll::Unjoin() Socket count:")+OSString(count));

					return(true);
				}

				int GetCount()const
				{
					return cur_count;
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

				int Update(double time_out)
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

					return num;
				}
            };//class SocketManageEpoll

            template<uint EVENT> class OnewaySocketManageEpoll:public SocketManageEpoll<OnewaySocketManageBase,EVENT>
            {
            public:

                using SocketManageEpoll<OnewaySocketManageBase,EVENT>::SocketManageEpoll;

				int Update(List<SocketEvent> &sock_list,List<SocketEvent> &error_list,double time_out)
				{
					const int num=SocketManageEpoll<OnewaySocketManageBase,EVENT>::Update(time_out);

					if(num<=0)
						return num;

					sock_list.SetCount(num);
					error_list.SetCount(num);

					epoll_event *ee=this->event_list;

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
						if(ee->events&EVENT)				//有事件了
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
            };//class OnewaySocketManageEpoll

            class TwowaySocketManageEpoll:public SocketManageEpoll<TwowaySocketManageBase,EPOLLIN|EPOLLOUT>
            {
            public:

                using SocketManageEpoll<TwowaySocketManageBase,EPOLLIN|EPOLLOUT>::SocketManageEpoll;

				int Update(List<SocketEvent> &recv_list,List<SocketEvent> &send_list,List<SocketEvent> &error_list,double time_out)
				{
					const int num=SocketManageEpoll<TwowaySocketManageBase,EPOLLIN|EPOLLOUT>::Update(time_out);

					if(num<=0)
						return num;

					recv_list.SetCount(num);
					send_list.SetCount(num);
					error_list.SetCount(num);

					epoll_event *ee=this->event_list;

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
						}
						else
						if(ee->events&EPOLLIN)				//可以读数据
						{
							rp->sock=ee->data.fd;
							rp->size=-1;
							++rp;
							++recv_num;
						}
						else
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
			};//class class TwowaySocketManageEpoll

			template<typename SMB>
			SMB *CreateSocketManageBaseEpoll(int max_user)
			{
				int epoll_fd=epoll_create(max_user);

				if(epoll_fd<0)
				{
//					std::cout<<"epoll_create return error,errno is "<<errno<<std::endl;
					return(nullptr);
				}

				return(new SMB(max_user,epoll_fd));
			}
		}//namespace

		OnewaySocketManageBase *CreateRecvSocketManage(int max_user)
		{
			return CreateSocketManageBaseEpoll<OnewaySocketManageEpoll<EPOLLIN>>(max_user);
		}

		OnewaySocketManageBase *CreateSendSocketManage(int max_user)
		{
			return CreateSocketManageBaseEpoll<OnewaySocketManageEpoll<EPOLLOUT>>(max_user);
		}

		TwowaySocketManageBase *CreateSocketManage(int max_user)
		{
			return CreateSocketManageBaseEpoll<TwowaySocketManageEpoll>(max_user);
		}
	}//namespace network
}//namespace hgl
