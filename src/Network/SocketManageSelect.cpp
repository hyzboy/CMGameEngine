#include"SocketManageBase.h"
#include<hgl/network/Socket.h>
#include<hgl/Time.h>
#include<hgl/type/Set.h>
#include<hgl/LogInfo.h>

namespace hgl
{    
	void SetTimeVal(timeval &tv,const double t_sec);

    namespace network
    {
		class SocketManageSelect:public SocketManageBase
		{
            int max_connect;
            int cur_count;
            
			int max_fd;

			Set<int> sock_id_list;

			fd_set	fd_sock_list;		//完整的sock列表

			fd_set	fd_recv_list;
			fd_set	fd_send_list;
            fd_set	fd_error_list;

			timeval time_out,*time_par;

		public:

			SocketManageSelect(int mc)
			{
                cur_count=0;
				max_connect=mc;
                max_fd=0;

				Clear();
			}

			bool Join(int sock) override
			{
				FD_SET(sock,&fd_sock_list);

				sock_id_list.Add(sock);

				if(sock>max_fd)
					max_fd=sock;
                
                SetSocketBlock(sock,false);

				cur_count++;

				return(true);
			}

			bool Unjoin(int sock) override
			{
				cur_count--;

				FD_CLR(sock,&fd_sock_list);

				sock_id_list.Delete(sock);

				return(true);
			}

            int GetCount()const override
            {
                return cur_count;
            }
            
			void Clear() override
			{
				cur_count=0;

				max_fd=0;

				FD_ZERO(&fd_sock_list);
				FD_ZERO(&fd_recv_list);
			}

            int Update(const double &to,SocketEventList &recv_list,SocketEventList &send_list,SocketEventList &error_list) override
			{
				if(cur_count<=0)
					return(0);

				if(to<=0)
					time_par=nullptr;		//传入nullptr表示阻塞模式
				else
				{						    //时间为0表示非阻塞
					SetTimeVal(time_out,to);

					time_par=&time_out;
				}

				fd_recv_list=fd_sock_list;

				if(select(max_fd+1,&fd_recv_list,&fd_send_list,&fd_error_list,time_par)<0)
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
                int error_num=0;
				{
					int count=sock_id_list.GetCount();
					int *id=sock_id_list.GetData();

                    recv_list.PreMalloc(count);
                    send_list.PreMalloc(count);
                    error_list.PreMalloc(count);
                    
                    SocketEvent *rp=recv_list.GetData();
                    SocketEvent *sp=send_list.GetData();
                    SocketEvent *ep=error_list.GetData();

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

                        if(FD_ISSET(*id,&fd_error_list))			//有错误
						{
							ep->sock=*id;
							ep->size=-1;
							++ep;
							++error_num;
						}

						++id;
					}
				}
                
                recv_list.SetCount(recv_num);
                send_list.SetCount(send_num);
                error_list.SetCount(error_num);

				return(recv_num+send_num+error_num);
			}
        };//class SocketManageSelect:public SocketManageBase
        
		SocketManageBase *CreateSelectSocketManageBase(int max_user)
		{
			return(new SocketManageSelect(max_user));
		}
    }//namespace network
}//namespace hgl
