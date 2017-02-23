#include<hgl/network/SocketManageThread.h>
#include<hgl/network/SocketManageBase.h>
#include<hgl/network/Socket.h>

namespace hgl
{
    namespace network
    {
        SocketManageThread::SocketManageThread()
        {
            sock_manage=nullptr;

            time_out=HGL_NETWORK_TIME_OUT;
        }

        SocketManageThread::~SocketManageThread()
        {
            SAFE_CLEAR(sock_manage);
        }

        bool SocketManageThread::Join(IOSocket *s)
        {
            if(!s)RETURN_FALSE;

            bool result;

            sock_join_set.Lock();
            result=(sock_join_set->Add(s)!=-1);
            sock_join_set.SemUnlock(result);

            return(result);
        }

        int SocketManageThread::Join(IOSocket **sl, const int count)
        {
            if(!sl||count<=0)
                RETURN_FALSE;

            int result;

            sock_join_set.Lock();
            result=sock_join_set->Add(sl,count);
            sock_join_set.SemUnlock(result);

            return result;
        }

        void SocketManageThread::ProcSocketJoin()
        {
            int count=sock_join_set->GetCount();
            IOSocket **sp=sock_join_set->GetData();

            for(int i=0;i<count;i++)
            {
                if((*sp)->ThisSocket!=-1)
                {
                    if(sock_set->Find((*sp)->ThisSocket)==-1)
                    {
                        if(sock_manage->Join((*sp)->ThisSocket))
                            sock_set->Add((*sp)->ThisSocket,*sp);
                    }
                }

                ++sp;
            }

            sock_join_set->ClearData();
            sock_join_set.Unlock();
        }

        bool SocketManageThread::Unjoin(IOSocket *s)
        {
            if(!s)RETURN_FALSE;
            if(s->ThisSocket==-1)RETURN_FALSE;

            sock_manage->Unjoin(s->ThisSocket);

            sock_set.Lock();
                sock_set->DeleteByIndex(s->ThisSocket);
            sock_set.Unlock();
            return(true);
        }

        int SocketManageThread::GetCount() const
        {
            return sock_manage->GetCount();
        }

        void SocketManageThread::Clear()
        {
            sock_manage->Clear();

            sock_set.Lock();
                sock_set->ClearData();
            sock_set.Unlock();
        }

        void SocketManageThread::ProcSocketDelete()
        {
            const int count=delete_list.GetCount();

            if(count>0)
            {
                sock_manage->Unjoin(delete_list.GetData(),count);
                sock_set->DeleteByIndex(delete_list.GetData(),count);

                delete_list.ClearData();
            }
        }

        bool SocketManageThread::Execute()
        {
            //只有这个里调用sock_manage，所以它可以不加锁使用

            if(sock_manage->GetCount()>0)        //如果当前有人，就不能卡住，所以走try_lock
            {
                if(sock_join_set.TrySemLock())
                {
                    sock_set.Lock();
                    ProcSocketJoin();
                }
                else
                    sock_set.Lock();
            }
            else                //当前没人，那就慢慢等吧
            {
                if(!sock_join_set.WaitSemLock(time_out))
                    return(true);
                else
                {
                    sock_set.Lock();
                    ProcSocketJoin();
                }
            }

            ProcSocketDelete();
            sock_set.Unlock();

            int num=sock_manage->Update(event_list,error_list,time_out);

            if(num<=0)
                return(true);

            if(event_list.GetCount()>0)
            {
                const int count=event_list.GetCount();
                SocketEvent *se_list=event_list.GetData();
                IOSocket *obj;

                for(int i=0;i<count;i++)
                {
                    if(sock_set->Get(se_list->sock,obj))
                    {
                        if(!ProcEvent(obj,se_list->size))
                            delete_list.Add(se_list->sock);
                    }
                    else
                        delete_list.Add(se_list->sock);

                    ++se_list;
                }

                event_list.ClearData();
            }

            if(error_list.GetCount()>0)
            {
                const int count=error_list.GetCount();
                SocketEvent *se_list=error_list.GetData();
                IOSocket *obj;

                for(int i=0;i<count;i++)
                {
                    if(sock_set->Get(se_list->sock,obj))
                        error_socket_list.Add(obj);

                    delete_list.Add(se_list->sock);

                    ++se_list;
                }

                error_list.ClearData();

                ProcError(error_socket_list.GetData(),error_socket_list.GetCount());
            }

            return(true);
        }
    }//namespace network
}//namespace hgl
