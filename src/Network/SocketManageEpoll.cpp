#include"SocketManageBase.h"
#include<hgl/network/Socket.h>
#include<hgl/LogInfo.h>

#include<unistd.h>
#include<sys/epoll.h>

namespace hgl
{
    namespace network
    {
        class SocketManageEpoll:public SocketManageBase
        {
        protected:

            int epoll_fd;
            uint user_event;

            int max_connect;
            int cur_count;

        protected:

            epoll_event *event_list;

        private:

            bool epoll_add(int sock)
            {
                epoll_event ev;

                hgl_zero(ev);

                ev.data.fd= sock;
                ev.events=  user_event  //要处理的事件
                            |EPOLLET    //边缘模式(即读/写时，需要一直读/写直到出错为止；相对LT模式是只要有数据就会一直通知)
                            |EPOLLERR   //出错
                            |EPOLLRDHUP //对方挂断
                            |EPOLLHUP;  //挂断

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

            SocketManageEpoll(int epfd,uint ue,int mc)
            {
                epoll_fd=epfd;
                user_event=ue;

                max_connect=mc;
                cur_count=0;

                event_list=new epoll_event[max_connect];
            }

            ~SocketManageEpoll()
            {
                delete[] event_list;

                close(epoll_fd);
            }

            bool Join(int sock) override
            {
                epoll_add(sock);

                SetSocketBlock(sock,false);

                ++cur_count;

                LOG_INFO(OS_TEXT("SocketManageEpoll::Join() Socket:")+OSString(sock));

                return(true);
            }

//             bool Join(const int *sock_list,int count) override
//             {
//                 if(count<=0)return(false);
//
//                 for(int i=0;i<count;i++)
//                 {
//                     epoll_add(*sock_list);
//
//                     SetSocketBlock(*sock_list,false);
//
//                     ++sock_list;
//                 }
//
//                 cur_count+=count;
//
//                 return(true);
//             }

            bool Unjoin(int sock) override
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

//             bool Unjoin(const int *sock_list,int count) override
//             {
//                 if(epoll_fd==-1)
//                 {
//                     LOG_ERROR(OS_TEXT("SocketManageEpoll::Unjoin() epoll_fd==-1)"));
//                     return(false);
//                 }
//
//                 if(!sock_list||count<=0)return(false);
//
//                 for(int i=0;i<count;i++)
//                 {
//                     epoll_del(*sock_list);
//                     ++sock_list;
//                 }
//
//                 cur_count-=count;
//                 LOG_INFO(OS_TEXT("SocketManageEpoll::Unjoin() Socket count:")+OSString(count));
//
//                 return(true);
//             }

            int GetCount()const override
            {
                return cur_count;
            }

            void Clear() override
            {
                if(epoll_fd!=-1)
                {
                    close(epoll_fd);
                    epoll_fd=-1;
                }

                cur_count=0;
            }

            int Update(const double &time_out,SocketEventList &recv_list,SocketEventList &send_list,SocketEventList &error_list) override
            {
                int event_count=0;

                if(epoll_fd==-1)
                    return(-1);

                if(cur_count<=0)
                    return(0);

                event_count=epoll_wait(epoll_fd,event_list,cur_count,time_out<0?-1:time_out*HGL_MILLI_SEC_PRE_SEC);

                if(event_count==0)
                    return(0);

                if(event_count<0)
                {
                    LOG_INFO(OS_TEXT("epoll return -1,errno: ")+OSString(errno));

                    if(errno==EBADF
                     ||errno==EFAULT
                     ||errno==EINVAL)
                        return(-1);

                    return(0);
                }

                recv_list.PreMalloc(event_count);
                send_list.PreMalloc(event_count);
                error_list.PreMalloc(event_count);

                epoll_event *ee=this->event_list;

                SocketEvent *rp=recv_list.GetData();
                SocketEvent *sp=send_list.GetData();
                SocketEvent *ep=error_list.GetData();

                int recv_num=0;
                int send_num=0;
                int error_num=0;

                for(int i=0;i<event_count;i++)
                {
                    if(ee->events&( EPOLLERR|           //出错了
                                    EPOLLRDHUP|         //对方关了
                                    EPOLLHUP))          //我方强制关了
                    {
                        LOG_ERROR("SocketManageEpoll Error,socket:"+OSString(ee->data.fd)+",epoll event:"+OSString(ee->events));

                        ep->sock=ee->data.fd;
                        ep->error=ee->events;
                        ++ep;
                        ++error_num;
                    }
                    else
                    if(ee->events&EPOLLIN)              //可以读数据
                    {
                        rp->sock=ee->data.fd;
                        rp->size=0;
                        ++rp;
                        ++recv_num;
                    }
                    else
                    if(ee->events&EPOLLOUT)             //可以发数据
                    {
                        sp->sock=ee->data.fd;
                        sp->size=0;
                        ++sp;
                        ++send_num;
                    }

                    ++ee;
                }

                recv_list.SetCount(recv_num);
                send_list.SetCount(send_num);
                error_list.SetCount(error_num);

                return(event_count);
            }
        };//class SocketManageEpoll:public SocketManageBase

        SocketManageBase *CreateSocketManageBase(int max_user)
        {
            if(max_user<=0)return(nullptr);

            int epoll_fd=epoll_create(max_user);

            if(epoll_fd<0)
            {
                LOG_ERROR(OS_TEXT("epoll_create return error,errno is")+OSString(errno));
                return(nullptr);
            }

            return(new SocketManageEpoll(epoll_fd,EPOLLIN,max_user));           //暂时只处理recv。。。至于send则直接强制发完，不走epoll
            //return(new SocketManageEpoll(epoll_fd,EPOLLIN|EPOLLOUT,max_user));
        }
    }//namespace network
}//namespace hgl
