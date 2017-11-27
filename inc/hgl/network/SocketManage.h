#ifndef HGL_NETWORK_SOCKET_MANAGE_INCLUDE
#define HGL_NETWORK_SOCKET_MANAGE_INCLUDE

#include<hgl/type/Map.h>
#include<hgl/thread/Thread.h>
#include<hgl/thread/SwapData.h>
#include<hgl/network/SocketEvent.h>
#include<hgl/network/SocketManageBase.h>
namespace hgl
{
    namespace network
    {
        class SocketManageBase;

        /**
         * 最简单的Socket管理类，直接在一个Update内处理socket的轮循、接收、发送<br>
         * 该类所有函数均为非线程安全，所以不可以直接在多线程中使用
         */
        template<typename USER_SOCKET> class SocketManage
        {
        protected:

            using SocketList=Map<int,USER_SOCKET *>;

            SocketList socket_list;

            SocketManageBase *manage;                                           ///<实际的Socket管理器

            SocketEventList sock_recv_list,
                            sock_send_list,
                            sock_error_list;

            Set<USER_SOCKET *> error_list;

        protected:

            void ProcSocketRecvList()
            {
                const int count=sock_recv_list.GetCount();
                SocketEvent *se=sock_recv_list.GetData();
                USER_SOCKET *sock;

                for(int i=0;i<count;i++)
                {
                    if(socket_list.Get(se->sock,sock))
                    {
                        if(sock->OnRecv(-1,0)<0)
                            error_list.Add(sock);
                    }
                    else
                    {
                        LOG_ERROR(OS_TEXT("Fatal error,can't find socket in SocketList,sock is ")+OSString(se->sock));
                    }

                    ++se;
                }
            }

            void ProcSocketSendList()
            {
                const int count=sock_send_list.GetCount();
                SocketEvent *se=sock_send_list.GetData();
                USER_SOCKET *sock;

                for(int i=0;i<count;i++)
                {
                    if(socket_list.Get(se->sock,sock))
                    {
                        if(sock->OnSend(-1)<0)
                            error_list.Add(sock);
                    }
                    else
                    {
                        LOG_ERROR(OS_TEXT("Fatal error,can't find socket in SocketList,sock is ")+OSString(se->sock));
                    }

                    ++se;
                }
            }

            void ProcSocketErrorList()
            {
                const int count=sock_error_list.GetCount();
                SocketEvent *se=sock_error_list.GetData();
                USER_SOCKET *sock;

                for(int i=0;i<count;i++)
                {
                    if(socket_list.Get(se->sock,sock))
                    {
                        sock->OnError(se->error);
                        error_list.Add(sock);
                    }
                    else
                    {
                        LOG_ERROR(OS_TEXT("Fatal error,can't find socket in SocketList,sock is ")+OSString(se->sock));
                    }

                    ++se;
                }
            }

            void ProcUpdateList()
            {
                const int count=socket_list.GetCount();
                auto **p=socket_list.GetDataList();

                for(int i=0;i<count;i++)
                {
                    if(!OnUpdate((*p)->right))
                        error_list.Add((*p)->right);

                    ++p;
                }
            }

            void ProcErrorList()
            {
                const int count=error_list.GetCount();
                USER_SOCKET **sp=error_list.GetData();

                for(int i=0;i<count;i++)
                {
                    Unjoin(*sp);
                    ++sp;
                }
            }

        public:

            SocketManage(int max_user)
            {
                manage=CreateSocketManageBase(max_user);
            }

            virtual ~SocketManage()
            {
                delete manage;
            }

            bool Join(USER_SOCKET *s)
            {
                if(!s)return(false);

                if(!socket_list.Add(s->ThisSocket,s))
                {
                    LOG_ERROR(OS_TEXT("repeat append socket to manage,sock:")+OSString(s->ThisSocket));
                    return(false);
                }

                if(!manage->Join(s->ThisSocket))
                {
                    socket_list.DeleteByIndex(s->ThisSocket);
                    return(false);
                }

                return(true);
            }

            int Join(USER_SOCKET **s_list,int count)
            {
                if(!s_list||count<=0)
                    return(-1);

                int total=0;

                for(int i=0;i<count;i++)
                {
                    if(this->Join(s_list[i]))
                        ++total;
                }

                return total;
            }

            bool Unjoin(USER_SOCKET *s)
            {
                if(!s)return(false);

                if(!socket_list.DeleteByIndex(s->ThisSocket))
                {
                    LOG_ERROR(OS_TEXT("socket don't in SocketManage,sock:")+OSString(s->ThisSocket));
                    return(false);
                }

                manage->Unjoin(s->ThisSocket);      //unjoin理论上不存在失败

                return(true);
            }

            int Unjoin(USER_SOCKET **s_list,int count)
            {
                if(!s_list||count<=0)
                    return(-1);

                int total=0;

                for(int i=0;i<count;i++)
                {
                    if(this->Unjoin(s_list[i]))
                        ++total;
                }

                return total;
            }

            virtual int Update(const double &time_out=HGL_NETWORK_TIME_OUT)
            {
                const int count=manage->Update(time_out,sock_recv_list,sock_send_list,sock_error_list);

                if(count<=0)
                    return(count);

                error_list.ClearData();

                ProcSocketSendList();       //这是上一帧的，所以先发。未来可能考虑改成另建一批线程发送
                ProcSocketRecvList();
                ProcUpdateList();
                ProcSocketErrorList();
                ProcErrorList();

                return count;
            }

        public:

            virtual void Clear()
            {
                const int count=socket_list.GetCount();
                auto **us=socket_list.GetDataList();

                for(int i=0;i<count;i++)
                {
                    (*us)->right->OnClose();
                    delete (*us)->right;           //未来版本不在这里delete，改为单开一个线程慢慢delete

                    ++us;
                }

                socket_list.ClearData();
            }

            virtual bool OnUpdate(USER_SOCKET *)=0;
        };//class SocketManage

        /**
         * 简单的Socket管理器线程
         */
        template<typename USER_SOCKET,typename USER_SOCKET_MANAGE> class SocketManageThread:public Thread
        {
        public:

            using UserSocketList=List<USER_SOCKET *>;                            ///<工作对象列表定义

        protected:

            USER_SOCKET_MANAGE *sock_manage;

        protected:

            SemSwapData<UserSocketList> join_list;                              ///<待添加的Socket对象列表
            SemSwapData<UserSocketList> unjoin_list;                            ///<待移出的Socket对象列表

            virtual void ClearUserSocket(USER_SOCKET *us)
            {
                delete us;
            }

            template<typename ST>
            void ClearUserSocketList(ST &sl)
            {
                const int count=sl.GetCount();
                USER_SOCKET **us=sl.GetData();

                for(int i=0;i<count;i++)
                {
                    ClearUserSocket(*us);
                    ++us;
                }

                sl.ClearData();
            }

            bool Join(USER_SOCKET *us){return sock_manage->Join(us);}           ///<单个工作对象接入处理函数
            bool Unjoin(USER_SOCKET *us){return sock_manage->Unjoin(us);}       ///<单个工作对象退出处理函数

            void ProcJoinList()                                                 ///<处理要接入的工作对象列表
            {
                UserSocketList &usl=join_list.GetReceive();

                const int count=usl.GetCount();
                USER_SOCKET **us=usl.GetData();

                for(int i=0;i<count;i++)
                {
                    Join(*us);
                    ++us;
                }

                usl.ClearData();
            }

            void ProcUnjoinList()                                               ///<处理要退出的工作对象列表
            {
                UserSocketList &usl=unjoin_list.GetReceive();

                const int count=usl.GetCount();
                USER_SOCKET **us=usl.GetData();

                for(int i=0;i<count;i++)
                {
                    Unjoin(*us);
                    ++us;
                }

                usl.ClearData();
            }

        public:

            SocketManageThread(USER_SOCKET_MANAGE *sm)
            {
                sock_manage=sm;
            }

            virtual ~SocketManageThread()
            {
                SAFE_CLEAR(sock_manage);
            }

            void ProcEndThread() override
            {
                ClearUserSocketList(join_list.GetReceive());
                join_list.Swap();
                ClearUserSocketList(join_list.GetReceive());

                sock_manage->Clear();

                //unjoin_list中的理论上都已经在wo_list/join_list里了，所以不需要走Clear，直接清空列表
                unjoin_list.GetReceive().ClearData();
                unjoin_list.Swap();
                unjoin_list.GetReceive().ClearData();
            }

            virtual bool Execute() override
            {
                if(join_list.TrySemSwap())
                    ProcJoinList();

                if(unjoin_list.TrySemSwap())
                    ProcUnjoinList();

                sock_manage->Update();
                return(true);
            }

        public:

            virtual UserSocketList &JoinBegin(){return join_list.GetPost();}    ///<开始添加要接入的Socket对象
            virtual void            JoinEnd()                                   ///<结束添加要接入的Socket对象
            {
                join_list.ReleasePost();
                join_list.PostSem();
            }

            virtual UserSocketList &UnjoinBegin(){return unjoin_list.GetPost();}///<开始添加要退出的Socket对象
            virtual void            UnjoinEnd()                                 ///<结束添加要退出的Socket对象
            {
                unjoin_list.ReleasePost();
                unjoin_list.PostSem();
            }
        };//template<typename USER_SOCKET,typename USER_SOCKET_MANAGE> class SocketManageThread:public Thread
    }//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SOCKET_MANAGE_INCLUDE
