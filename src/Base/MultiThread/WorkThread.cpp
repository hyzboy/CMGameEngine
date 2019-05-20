#include<hgl/thread/WorkThread.h>

namespace hgl
{
    namespace workteam
    {
        void WorkThread::ProcJoinList()
        {
            WorkObjectList &wol=join_list.GetReceive();

            const int count=wol.GetCount();
            WorkObject **wo=wol.GetData();

            for(int i=0;i<count;i++)
            {
                Join(*wo);
                ++wo;
            }

            wol.ClearData();
        }

        void WorkThread::ProcUnjoinList()
        {
            WorkObjectList &wol=unjoin_list.GetReceive();

            const int count=wol.GetCount();
            WorkObject **wo=wol.GetData();

            for(int i=0;i<count;i++)
            {
                Unjoin(*wo);
                ++wo;
            }

            wol.ClearData();
        }

        void WorkThread::UpdateWorkObjectSet()
        {
            const int count=wo_set.GetCount();
            WorkObject **wo=wo_set.GetData();

            for(int i=0;i<count;i++)
            {
                (*wo)->Update();
                ++wo;
            }
        }

        void WorkThread::ClearWorkObjectArray(WorkObject **wo,const int count)
        {
            for(int i=0;i<count;i++)
            {
                ClearWorkObject(*wo);
                ++wo;
            }
        }

        void WorkThread::ProcEndThread()
        {
            ClearWorkObjectList(join_list.GetReceive());
            join_list.Swap();
            ClearWorkObjectList(join_list.GetReceive());

            ClearWorkObjectArray(wo_set.GetData(),wo_set.GetCount());
            wo_set.ClearData();

            //unjoin_list中的理论上都已经在wo_list/join_list里了，所以不需要走Clear，直接清空列表
            unjoin_list.GetReceive().ClearData();
            unjoin_list.Swap();
            unjoin_list.GetReceive().ClearData();
        }

        bool WorkThread::Execute()
        {
            const int count=wo_set.GetCount();
            bool have;

            if(count<=0)
                have=join_list.WaitSemSwap();
            else
                have=join_list.TrySemSwap();

            if(have)
                ProcJoinList();

            have=unjoin_list.TrySemSwap();

            if(have)
                ProcUnjoinList();

            UpdateWorkObjectSet();
            return(true);
        }
    }//namespace workteam
}//namespace hgl
