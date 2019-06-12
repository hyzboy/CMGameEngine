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

            fd_set  fd_sock_list;       //完整的sock列表

            fd_set  fd_recv_list;
            fd_set  fd_send_list;
            fd_set  fd_error_list;

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

                LOG_INFO(OS_TEXT("Join ")+OSString(sock)+OS_TEXT(" to SocketManageSelect"));

                return(true);
            }

            bool Unjoin(int sock) override
            {
                cur_count--;

                FD_CLR(sock,&fd_sock_list);

                sock_id_list.Delete(sock);

                LOG_INFO(OS_TEXT("Unjoin ")+OSString(sock)+OS_TEXT(" from SocketManageSelect"));

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
                FD_ZERO(&fd_send_list);
                FD_ZERO(&fd_error_list);
            }

            int ConvertList(SocketEventList &sel,const fd_set &fs)
            {
                sel.SetCount(fs.fd_count);

                SocketEvent *p=sel.GetData();

                for(uint i=0;i<fs.fd_count;i++)
                {
                    p->sock=fs.fd_array[i];
                    p->size=-1;
                    ++p;
                }

                return fs.fd_count;
            }

            int Update(const double &to,SocketEventList &recv_list,SocketEventList &send_list,SocketEventList &error_list) override
            {
                if(cur_count<=0)
                    return(0);

                if(to<=0)
                    time_par=nullptr;       //传入nullptr表示阻塞模式
                else
                {                           //时间为0表示非阻塞
                    SetTimeVal(time_out,to);

                    time_par=&time_out;
                }

                memcpy(&fd_recv_list,   &fd_sock_list,sizeof(fd_sock_list));
                memcpy(&fd_send_list,   &fd_sock_list,sizeof(fd_sock_list));
                memcpy(&fd_error_list,  &fd_sock_list,sizeof(fd_sock_list));

                if(select(max_fd+1,&fd_recv_list,&fd_send_list,&fd_error_list,time_par)<0)
                {
                    LOG_INFO(OS_TEXT("select return -1,errno: ")+OSString(errno));

                    if(errno==EBADF
                     ||errno==EFAULT
                     ||errno==EINVAL)
                        return(-1);

                    return(0);
                }

                ConvertList(recv_list,fd_recv_list);
                ConvertList(send_list,fd_send_list);
                ConvertList(error_list,fd_error_list);

                return(fd_recv_list.fd_count
                      +fd_send_list.fd_count
                      +fd_error_list.fd_count);
            }
        };//class SocketManageSelect:public SocketManageBase

        SocketManageBase *CreateSocketManageBase(int max_user)
        {
            return(new SocketManageSelect(max_user));
        }
    }//namespace network
}//namespace hgl
