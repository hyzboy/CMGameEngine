#include<hgl/ut/TaskManager.h>
#include<hgl/LogInfo.h>

namespace hgl
{
    namespace util
    {
        TaskManager::TaskManager()
        {
            hglSetProperty(    Enabled,this,TaskManager::GetEnabled,TaskManager::SetEnabled);
            hglSetProperty(    CurTask,this,TaskManager::GetCurTask,TaskManager::SetCurTask);

            enabled=true;
            cur=-1;

            OnEnd=nullptr;
            OnChange=nullptr;
        }

        TaskManager::~TaskManager()
        {
            Clear();
        }

        void TaskManager::ChangeTask(int index)
        {
            SafeCallEvent(OnChange,(this,cur,index));
            cur=index;
        }

        /**
        * 开始运行任务管理器中的任务，注意它会从最早的任务开始
        * @return  是否开始运行
        */
        bool TaskManager::Start()
        {
            if(count>0)
            {
                ChangeTask(0);
                enabled=true;
                return(true);
            }
            #ifdef _DEBUG
                LOG_ERROR(OS_TEXT("开始运行任务失败，任务管理器中不存在任何任务！"));
            #endif//

            enabled=false;
            return(false);
        }

        /**
        * 暂停任务管理器的运行<br>
        * 它会将任务管理器状态设为rsPause
        */
        void TaskManager::Pause()
        {
            if(count)
                enabled=false;
        }

        /**
        * 将暂停中的任务管理器重新开始运行起来，如果不处于暂停状态，它会调用Start开始运行
        */
        void TaskManager::Resume()
        {
            if(enabled)
            {
                LOG_ERROR(OS_TEXT("任务管理器并没有暂停，程序会自动将Resume转为Start，请注意这可能是个程序错误！"));
                Start();
            }
            else
                enabled=true;
        }

        /**
        * 中止当前任务，直接跳到上一任务
        * @return 是否成功
        */
        bool TaskManager::Prev()
        {
            if(!enabled)return(false);

            if(cur>0)
            {
                ChangeTask(cur-1);
                return(true);
            }

            return(false);
        }

        /**
        * 中止当前任务，直接跳到下一任务
        * @return 是否成功
        */
        bool TaskManager::Next()
        {
            if(!enabled)return(false);

            ChangeTask(cur+1);

            if(cur>=count)
            {
                Stop();
                return(true);
            }

            return(false);
        }

        /**
        * 跳转到指定任务
        * @param index 任务索引
        * @return 是否成功
        */
        bool TaskManager::Goto(int index)
        {
            if(index==cur
                ||index<0
                ||index>=count)return(false);

            ChangeTask(index);
            return(true);
        }

        /**
        * 停止整个任务管理器的运行
        */
        void TaskManager::Stop()
        {
            ChangeTask(-1);
            enabled=false;
            SafeCallEvent(OnEnd,(this));
        }

        /**
        * 刷新并执行任务
        * @return 任务返回结果
        */
        bool TaskManager::Update()
        {
            if(!enabled)return(false);
            if(items[cur]==nullptr)return(false);

            return items[cur](this);
        }
    }//namespace util
}//namespace hgl
