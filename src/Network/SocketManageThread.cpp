#include<hgl/network/SocketManageThread.h>
namespace hgl
{
    namespace network
    {
        void SocketManageThread::ProcJoinList()
        {
            AcceptSocketList &usl=join_list.GetReceive();

            const int count=usl.GetCount();
            TCPAccept **us=usl.GetData();

            for(int i=0;i<count;i++)
            {
                Join(*us);
                ++us;
            }

            usl.ClearData();
        }

        void SocketManageThread::ProcUnjoinList()
        {
            AcceptSocketList &usl=unjoin_list.GetReceive();

            const int count=usl.GetCount();
            TCPAccept **us=usl.GetData();

            for(int i=0;i<count;i++)
            {
                Unjoin(*us);
                ++us;
            }

            usl.ClearData();
        }

        void SocketManageThread::ProcEndThread()
        {
            ClearAcceptSocketList(join_list.GetReceive());
            join_list.Swap();
            ClearAcceptSocketList(join_list.GetReceive());

            sock_manage->Clear();

            //unjoin_list中的理论上都已经在wo_list/join_list里了，所以不需要走Clear，直接清空列表
            unjoin_list.GetReceive().ClearData();
            unjoin_list.Swap();
            unjoin_list.GetReceive().ClearData();
        }

        bool SocketManageThread::Execute()
        {
            if(join_list.TrySemSwap())
                ProcJoinList();

            if(unjoin_list.TrySemSwap())
                ProcUnjoinList();

            sock_manage->Update();
            return(true);
        }
    }//namespace network
}//namespace hgl
