#include<hgl/thread/WorkThreadTeam.h>
#include<hgl/thread/WorkThread.h>

namespace hgl
{
    namespace workteam
    {
        bool WorkThreadTeam::AddWorkThread(WorkThread *wt)
        {
            if(!wt)return(false);

            return(wt_list.Add(wt)!=-1);
        }

        int WorkThreadTeam::InitWorkThread(int count)                                                           ///<初始化工作线程
        {
            if(count<=0)return(0);

            int total=0;
            WorkThread *wt;

            for(int i=0;i<count;i++)
            {
                wt=CreateWorkThread();

                if(!wt)
                    break;

                wt_list.Add(wt);
                ++total;
            }

            return total;
        }

        int WorkThreadTeam::StartWorkThread()
        {
            const int count=wt_list.GetCount();

            if(count<=0)return count;

            int total=0;
            WorkThread **wt=wt_list.GetData();

            for(int i=0;i<count;i++)
            {
                if((*wt)->Start())
                    ++total;

                ++wt;
            }

            return total;
        }

        bool WorkThreadTeam::Join(WorkObject *wo)
        {
            if(!wo)return(false);

            WorkThread *wt;

            OnlyWriteLock owl(o4t);
            {
                if(o4t->KeyExist(wo))
                    return(false);

                wt=DistWorkThread(wo);

                if(!wt)
                    return(false);

                o4t->Add(wo,wt);
                owl.Unlock();
            }

            WorkObjectList &wol=wt->JoinBegin();

            wol.Add(wo);

            wt->JoinEnd();

            return(true);
        }

        int WorkThreadTeam::Join(WorkObject **wo_list,int wo_count)
        {
            if(!wo_list)return(-1);
            if(wo_count<=0)return(-2);

            WorkThread *wt=DistWorkThread(nullptr);

            if(!wt)return(false);

            int total=0;

            WorkObjectList &wol=wt->JoinBegin();

            o4t.WriteLock();

            for(int i=0;i<wo_count;i++)
            {
                if(o4t->KeyExist(wo_list[i]))
                    continue;

                o4t->Add(wo_list[i],wt);
                wol.Add(wo_list[i]);
                ++total;
            }

            o4t.WriteUnlock();

            wt->JoinEnd();

            return(true);
        }

        bool WorkThreadTeam::Unjoin(WorkObject *wo)
        {
            if(!wo)return(false);

            WorkThread *wt;

            OnlyWriteLock owl(o4t);
            {
                if(!o4t->Delete(wo,wt))
                    return(false);

                owl.Unlock();
            }

            WorkObjectList &wol=wt->UnjoinBegin();

            wol.Add(wo);

            wt->UnjoinEnd();

            return(true);
        }
    }//namespace workteam
}//namespace hgl
