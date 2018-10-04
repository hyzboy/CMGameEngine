#include<hgl/thread/Thread.h>
#include<hgl/LogInfo.h>

namespace hgl
{
    extern THREAD_FUNC ThreadFunc(Thread *tc);

    /**
    * (线程外部调用)执行当前线程，线程优先级为tplevel
    * @param tplevel 线程优先级
    * @return 是否创建线程成功
    */
    bool Thread::Start()
    {
        unsigned long threadid;

        exit_lock.Lock();

        tp=::CreateThread(0,0,(PTHREAD_START_ROUTINE)ThreadFunc,this,0,&threadid);

        if(!tp)
        {
            exit_lock.Unlock();
            LOG_ERROR(OS_TEXT("Create Thread failed,Windows ErrorCode: ")+OSString((uint)GetLastError()));
            return(false);
        }

        return(true);
    }

    /**
    * 是否是当前线程
    */
    bool Thread::IsCurThread()
    {
        return(tp==GetCurrentThread());
    }

    void WaitThreadExit(thread_ptr tp,const double &time_out)
    {
        if(!tp)return;

        WaitForSingleObject(tp,time_out>0?time_out*1000:INFINITE);
    }

    /**
    * 等待多个线程中的一个完成
    * @param mt 线程
    * @param count 线程数量
    * @param time_out 等待的时间，如果为0表示等到线程运行结束为止。默认为0
    */
    void WaitThread(Thread **mt,int count,double time_out)
    {
        void **obj=new void *[count];

        for(int i=0;i<count;i++)
            obj[i]=mt[i]->tp;

        WaitForMultipleObjects(count,obj,false,time_out>0?time_out*1000:INFINITE);

        delete[] obj;
    }
}//namespace hgl
