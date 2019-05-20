#ifndef HGL_WORKTEAM_WORK_THREAD_INCLUDE
#define HGL_WORKTEAM_WORK_THREAD_INCLUDE

#include<hgl/type/Set.h>
#include<hgl/type/List.h>
#include<hgl/thread/Thread.h>
#include<hgl/thread/SwapData.h>
#include<hgl/thread/WorkObject.h>
namespace hgl
{
    /**
     * 多线程工作组名字空间
     */
    namespace workteam
    {
        using WorkObjectList=List<WorkObject *>;                                ///<工作对象列表定义

        /**
         * 具体工作线程<Br>
         * 用于管理多个WorkObject，并不停的调用所有WorkObject::Update
         */
        class WorkThread:public Thread
        {
        protected:

            Set<WorkObject *> wo_set;                                           ///<工作对象合集

            SemSwapData<WorkObjectList> join_list;                              ///<待添加的工作对象列表
            SemSwapData<WorkObjectList> unjoin_list;                            ///<待移出的工作对象列表

        protected:

            virtual bool Join(WorkObject *wo){return(wo_set.Add(wo)!=-1);}      ///<单个工作对象接入处理函数
            virtual bool Unjoin(WorkObject *wo){return wo_set.Delete(wo);}      ///<单个工作对象退出处理函数

            virtual void ProcJoinList();                                        ///<处理要接入的工作对象列表
            virtual void ProcUnjoinList();                                      ///<处理要退出的工作对象列表

            virtual void UpdateWorkObjectSet();                                 ///<刷新工作对象合集

            virtual void ClearWorkObject(WorkObject *wo)                        ///<对象清理函数，如需其它处理方式请重载
            {
                if(!wo)
                    delete wo;
            }

                    void ClearWorkObjectArray(WorkObject **,int);

                    void ClearWorkObjectList(WorkObjectList &wol)
                    {
                        ClearWorkObjectArray(wol.GetData(),wol.GetCount());
                        wol.ClearData();
                    }

        protected:  //线程相关处理重载

            virtual void ProcEndThread() override;
            virtual bool Execute() override;

        public:

            WorkThread()=default;
            virtual ~WorkThread()=default;

            virtual WorkObjectList &JoinBegin(){return join_list.GetPost();}    ///<开始添加要接入的工作对象
            virtual void            JoinEnd()                                   ///<结束添加要接入的工作对象
            {
                join_list.ReleasePost();
                join_list.PostSem();
            }

            virtual WorkObjectList &UnjoinBegin(){return unjoin_list.GetPost();}///<开始添加要退出的工作对象
            virtual void            UnjoinEnd()                                 ///<结束添加要退出的工作对象
            {
                unjoin_list.ReleasePost();
                unjoin_list.PostSem();
            }
        };//class WorkThread
    }//namespace workteam
}//namespace hgl
#endif//HGL_WORKTEAM_WORK_THREAD_INCLUDE
