#ifndef HGL_THREAD_ASYNC_EVENT_INCLUDE
#define HGL_THREAD_ASYNC_EVENT_INCLUDE

#include<hgl/thread/SwapData.h>
#include<hgl/type/Queue.h>
#include<hgl/thread/Thread.h>
#include<hgl/Time.h>

namespace hgl
{
    namespace async
    {
        /**
         * 事件基类
         */
        class EventProc
        {
        public:

            EventProc()=default;
            virtual ~EventProc()=default;

            virtual bool Proc(const double &cur_time)=0;
        };//class EventProc

        using EventProcQueue=Queue<EventProc *>;
        using MTEventProcQueue=SemSwapData<EventProcQueue>;
        using MTEventProcPost=PostToSemSwapData<EventProcQueue>;                  ///<多线程事件结果投递对象重定义

        /**
         * 增加事件到队列
         * @param queue 事件队列
         * @param event 事件
         */
        inline void AddToEventQueue(MTEventProcQueue *queue,EventProc *event)
        {
            if(!queue||!event)
                return;

            MTEventProcPost post(queue);

            post->Push(event);
        }

        /**
         * 刷新执行事件队列
         * @param proc_queue 要执行的事件队列
         * @param cur_time 当前时间
         * @param wait 是否等待有事件
         */
        inline void UpdateEventProcQueue(MTEventProcQueue *proc_queue,const double &cur_time,bool wait=false)
        {
            if(!proc_queue)
                return;

            if(wait)
            {
                if(!proc_queue->WaitSemSwap())
                    return;
            }
            else
            {
                if(!proc_queue->TrySemSwap())
                    return;
            }

            EventProcQueue &epq=proc_queue->GetReceive();

            if(epq.GetCount()<=0)
                return;

            EventProc *event;

            while(epq.Pop(event))
            {
                event->Proc(cur_time);
                delete event;
            }
        }

        /**
         * 事件执行线程
         */
        class EventThread:public Thread
        {
            MTEventProcQueue *event_proc_queue;

        public:

            EventThread(MTEventProcQueue *queue):event_proc_queue(queue){}
            virtual ~EventThread()=default;

            bool Execute()
            {
                if(!queue)
                    return(false);

                UpdateEventProcQueue(event_proc_queue,GetDoubleTime(),true);

                return(true);
            }
        };//class RepoManageThread:public Thread
    }//namespace async
}//namespace hgl
#endif//HGL_THREAD_ASYNC_EVENT_INCLUDE
