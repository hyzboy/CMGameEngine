#include<hgl/network/MTSocketManage.h>

namespace hgl
{
    namespace network
    {
        MTSocketManage::MTSocketManage()
        {
            max_socket_count=0;
            recv_thread_count=0;
            send_thread_count=0;

            recv_manage=nullptr;
            send_manage=nullptr;
        }

        MTSocketManage::~MTSocketManage()
        {
            SAFE_CLEAR_OBJECT_ARRAY(recv_manage,recv_thread_count);
            SAFE_CLEAR_OBJECT_ARRAY(send_manage,send_thread_count);
        }

        /**
        * 初始化多线程Socket I/O管理器
        * @param _max_socket_count 最大socket数量
        * @param _recv_thread_count 接收线程数
        * @param _send_thread_count 发送线程数(默认为和接收线程数量一样)
        * @return 是否初始化成功
        */
        bool MTSocketManage::Init(int _max_socket_count,int _recv_thread_count,int _send_thread_count)
        {
            if(_max_socket_count<=0)return(false);
            if(_recv_thread_count<=0)return(false);

            max_socket_count=_max_socket_count;
            recv_thread_count=_recv_thread_count;

            if(_send_thread_count<=0)
                send_thread_count=_recv_thread_count;
            else
                send_thread_count=_send_thread_count;

            recv_manage=new SocketManageThread *[recv_thread_count];
            send_manage=new SocketManageThread *[send_thread_count];

            for(int i=0;i<recv_thread_count;i++)
                recv_manage[i]=CreateRecvSocketManageThread();

            for(int i=0;i<send_thread_count;i++)
                send_manage[i]=CreateSendSocketManageThread();

            return(true);
        }

        bool MTSocketManage::StartRecvThread()
        {
            for(int i=0;i<recv_thread_count;i++)
                if(!recv_manage[i]->Start())
                    return(false);

            return(true);
        }

        bool MTSocketManage::StartSendThread()
        {
            for(int i=0;i<send_thread_count;i++)
                if(!send_manage[i]->Start())
                    return(false);

            return(true);
        }

        bool MTSocketManage::Join(IOSocket *s)                                                                    ///<增加一个Socket
        {
            if(!s)return(false);

            int result;

            sock_set.Lock();
                result=sock_set->Add(s);
            sock_set.Unlock();

            if(result==-1)
            {
                LOG_ERROR(OS_TEXT("MTSocketManage::Join() Repeat join socket:")+OSString(s->ThisSocket));
                return(false);
            }

            if(!recv_manage[s->ThisSocket%recv_thread_count]->Join(s))
                return(false);

            if(!send_manage[s->ThisSocket%send_thread_count]->Join(s))
            {
                recv_manage[s->ThisSocket%recv_thread_count]->Unjoin(s);
                return(false);
            }

            return(true);
        }

        bool MTSocketManage::Unjoin(IOSocket *s)                                                                ///<移除一个Socket
        {
            if(!s)return(false);

            bool result;

            sock_set.Lock();
                result=sock_set->Delete(s);
            sock_set.Unlock();

            if(!result)
            {
                LOG_ERROR(OS_TEXT("MTSocketManage::Unjoin() don't find this socket:")+OSString(s->ThisSocket));
                return(false);
            }

            recv_manage[s->ThisSocket%recv_thread_count]->Unjoin(s);
            send_manage[s->ThisSocket%send_thread_count]->Unjoin(s);

            return(true);
        }

        void MTSocketManage::ProcError(SocketManageThread *smt,IOSocket **sock_list,const int count)
        {
            if(!sock_list||count<=0)return;

            error_sock_set.Add(sock_list,count);
        }

        bool MTSocketManage::Execute()
        {
            if(!error_sock_set.WaitProc())
                return(true);

            const int count=error_sock_set->GetCount();
            IOSocket **sp=error_sock_set->GetData();

            sock_set.Lock();
                sock_set->Delete(sp,count);
            sock_set.Unlock();

            for(int i=0;i<count;i++)
            {
                ProcError(*sp);

                ++sp;
            }

            error_sock_set->ClearData();

            return(true);
        }
    }//namespace network
}//namespace hgl
