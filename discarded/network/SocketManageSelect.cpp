#include<hgl/network/SocketManageBase.h>
#include<hgl/type/Set.h>
#include<hgl/LogInfo.h>

#include<unistd.h>
#include<sys/epoll.h>
#include<iostream>

namespace hgl
{
	void SetTimeVal(timeval &tv,const double t_sec);

	namespace network
	{
		class SocketManageSelect:public SocketManageBase
		{
			int					max_connect;
			atom_int			cur_count;

			int max_fd;

			Set<int> sock_id_list;

			fd_set	fd_sock_list;		//完整的sock列表

			fd_set	fd_recv_list;
			fd_set	fd_send_list;

			timeval time_out,*time_par;

		public:

			SocketManageSelect(int mc)
			{
				max_connect=mc;

				Clear();
			}

			~SocketManageSelect()
			{
			}

			bool Join(IOSocket *sock)
			{
				FD_SET(sock->ThisSocket,&fd_sock_list);

				sock_id_list.Add(sock->ThisSocket);

				if(sock->ThisSocket>max_fd)
					max_fd=sock->ThisSocket;

				sock->SetBlock(false);

				cur_count++;

				return(true);
			}

			bool Unjoin(IOSocket *sock)
			{
				cur_count--;

				FD_CLR(sock->ThisSocket,&fd_sock_list);

				sock_id_list.Delete(sock->ThisSocket);

				return(true);
			}

			bool Change(IOSocket *sock,bool r,bool w)
			{
			}

			void Clear()
			{
				cur_count=0;

				max_fd=0;

				FD_ZERO(&fd_sock_list);
				FD_ZERO(&fd_recv_list);
			}

			int Update(MemBlock<SocketEvent> &recv_list,MemBlock<SocketEvent> &send_list,double t)
			{
				if(cur_count<=0)
					return(0);

				if(t<=0)
					time_par=nullptr;		//传入NULL表示阻塞模式
				else
				{						//时间为0表示非阻塞
					SetTimeVal(time_out,t);

					time_par=&time_out;
				}

				fd_recv_list=fd_sock_list;

				if(select(max_fd+1,&fd_recv_list,&fd_send_list,nullptr/*error_list*/,time_par)<0)
				{
					LOG_INFO(OS_TEXT("select return -1,errno: ")+OSString(errno));

					if(errno==EBADF
					 ||errno==EFAULT
					 ||errno==EINVAL)
						return(-1);

					return(0);
				}

				int recv_num=0;
				int send_num=0;
				{
					int count=sock_id_list.GetCount();
					int *id=sock_id_list.GetData();

					SocketEvent *rp=recv_list.data();
					SocketEvent *sp=send_list.data();

					for(int i=0;i<count;i++)
					{
						if(FD_ISSET(*id,&fd_recv_list))				//读进了数据
						{
							rp->sock=*id;
							rp->size=-1;
							++rp;
							++recv_num;
						}

						if(FD_ISSET(*id,&fd_send_list))				//可以写数据
						{
							sp->sock=*id;
							sp->size=-1;
							++sp;
							++send_num;
						}

						++id;
					}
				}

				return(recv_num+send_num);
			}
		};//class SocketManageSelect

		SocketManageBase *CreateSelectSocketManageBase(int max_user)
		{
			return(new SocketManageSelect(max_user));
		}
	}//namespace network
}//namespace hgl
