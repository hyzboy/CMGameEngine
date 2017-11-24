#ifndef HGL_WORKTEAM_WORK_THREAD_TEAM_INCLUDE
#define HGL_WORKTEAM_WORK_THREAD_TEAM_INCLUDE

#include<hgl/type/Set.h>
#include<hgl/type/Map.h>
#include<hgl/thread/RWLock.h>
#include<hgl/thread/SwapData.h>

namespace hgl
{
    namespace workteam
    {
        class WorkObject;
        class WorkThread;

        /**
        * 多线程工作组管理器<br>
        * 用于管理多个WorkThread
        */
        class WorkThreadTeam
        {
        protected:

            Set<WorkThread *> wt_list;                                                              ///<工作线程列表

            int wt_poll_dist=0;

            RWLockObject<Map<WorkObject *,WorkThread *>> o4t;                                       ///<工作对象对应工作线程表

        public:

            WorkThreadTeam()=default;
            virtual ~WorkThreadTeam()=default;

            virtual WorkThread *CreateWorkThread()=0;                                               ///<创建工作线程纯虚函数
            virtual WorkObject *CreateWorkObject()=0;                                               ///<创建工作对象纯虚函数

            virtual WorkThread *DistWorkThread(WorkObject *)                                        ///<分配工作线程纯虚函数(注：此函数被多线程调用)
            {
                if(wt_list.GetCount()<=0)return(nullptr);

                int index=wt_poll_dist;         //wt_poll_dist在多线程中多次计算不安全，所以这样使用

                ++wt_poll_dist;                 //无需考虑越界

                index%=wt_list.GetCount();

                return wt_list[index];
            }

        public:

            bool AddWorkThread(WorkThread *wt);                                                     ///<增加一个工作线程到集合中
             int InitWorkThread(int count);                                                         ///<初始化工作线程

             int StartWorkThread();                                                                 ///<启动工作线程

        public:

            bool Join(WorkObject *);                                                                ///<加入一个工作对象到组中
             int Join(WorkObject **,int);                                                           ///<加入一批工作对象到组中
            bool Unjoin(WorkObject *);                                                              ///<从组中解除一个工作对象
             int Unjoin(WorkObject **,int);                                                         ///<从组中解除一批工作对象
        };//class WorkThreadTeam
    }//namespace workteam
}//namespace hgl
#endif//HGL_WORKTEAM_WORK_THREAD_TEAM_INCLUDE
