#include<hgl/network/SocketManage.h>
#include<hgl/LogInfo.h>
#include"SocketManageBase.h"

namespace hgl
{
    namespace network
    {
        SocketManage::SocketManage(int max_user)
        {
            manage=CreateSocketManageBase(max_user);
        }

        SocketManage::~SocketManage()
        {
            delete manage;
        }

        void SocketManage::ProcSocketRecvList()
        {
            const int count=sock_recv_list.GetCount();

            if(count<=0)return;

            SocketEvent *se=sock_recv_list.GetData();
            TCPAccept *sock;

            for(int i=0;i<count;i++)
            {
                if(socket_list.Get(se->sock,sock))
                {
                    if(sock->OnSocketRecv(se->error)<0)
                    {
                        LOG_INFO(OS_TEXT("OnSocketRecv return Error,sock:")+OSString(se->sock));
                        error_set.Add(sock);
                    }
                }
                else
                {
                    LOG_ERROR(OS_TEXT("Fatal error,can't find socket in SocketList,sock is ")+OSString(se->sock));
                }

                ++se;
            }

            sock_recv_list.ClearData();
        }

        void SocketManage::ProcSocketSendList()
        {
            const int count=sock_send_list.GetCount();

            if(count<=0)return;

            SocketEvent *se=sock_send_list.GetData();
            TCPAccept *sock;

            for(int i=0;i<count;i++)
            {
                if(socket_list.Get(se->sock,sock))
                {
                    if(sock->OnSocketSend(se->size)<0)
                    {
                        LOG_INFO(OS_TEXT("OnSocketSend return Error,sock:")+OSString(se->sock));
                        error_set.Add(sock);
                    }
                }
                else
                {
                    LOG_ERROR(OS_TEXT("Fatal error,can't find socket in SocketList,sock is ")+OSString(se->sock));
                }

                ++se;
            }

            sock_send_list.ClearData();
        }

        void SocketManage::ProcSocketErrorList()
        {
            const int count=sock_error_list.GetCount();

            if(count<=0)return;

            SocketEvent *se=sock_error_list.GetData();
            TCPAccept *sock;

            for(int i=0;i<count;i++)
            {
                if(socket_list.Get(se->sock,sock))
                {
                    LOG_INFO(OS_TEXT("SocketError,sock:")+OSString(se->sock)+OS_TEXT(",errno:")+OSString(se->error));
                    sock->OnSocketError(se->error);
                    error_set.Add(sock);
                }
                else
                {
                    LOG_ERROR(OS_TEXT("Fatal error,can't find socket in SocketList,sock is ")+OSString(se->sock));
                }

                ++se;
            }

            sock_error_list.ClearData();
        }

        void SocketManage::ProcErrorList()
        {
            const int count=error_set.GetCount();

            if(count<=0)return;

            TCPAccept **sp=error_set.GetData();

            for(int i=0;i<count;i++)
            {
                Unjoin(*sp);
                ++sp;
            }

            //error_set不在这里ClearData，在主循环的一开始，参见那里的注释
        }

        bool SocketManage::Join(TCPAccept *s)
        {
            if(!s)return(false);

            if(!socket_list.Add(s->ThisSocket,s))
            {
                LOG_ERROR(OS_TEXT("repeat append socket to manage,sock:")+OSString(s->ThisSocket));
                return(false);
            }

            if(!manage->Join(s->ThisSocket))
            {
                socket_list.DeleteByKey(s->ThisSocket);
                return(false);
            }

            return(true);
        }

        int SocketManage::Join(TCPAccept **s_list,int count)
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

        bool SocketManage::Unjoin(TCPAccept *s)
        {
            if(!s)return(false);

            if(!socket_list.DeleteByKey(s->ThisSocket))
            {
                LOG_ERROR(OS_TEXT("socket don't in SocketManage,sock:")+OSString(s->ThisSocket));
                return(false);
            }

            manage->Unjoin(s->ThisSocket);      //unjoin理论上不存在失败

            return(true);
        }

        int SocketManage::Unjoin(TCPAccept **s_list,int count)
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

        int SocketManage::Update(const double &time_out)
        {
            //将error_set放在这里，是为了保留它给外面的调用者使用
            error_set.ClearData();

            const int count=manage->Update(time_out,sock_recv_list,sock_send_list,sock_error_list);

            if(count<=0)
                return(count);

            ProcSocketSendList();       //这是上一帧的，所以先发。未来可能考虑改成另建一批线程发送
            ProcSocketRecvList();
            ProcSocketErrorList();
            ProcErrorList();            //这里仅仅是将Socket从列表中移除，并没有删掉。

            return count;
        }

        void SocketManage::Clear()
        {
            const int count=socket_list.GetCount();
            auto **us=socket_list.GetDataList();

            for(int i=0;i<count;i++)
            {
                Unjoin((*us)->right);

                ++us;
            }

            socket_list.ClearData();
        }
    }//namespace network
}//namespace hgl
