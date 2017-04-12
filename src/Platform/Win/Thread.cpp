#include<hgl/thread/Thread.h>
#include<hgl/LogInfo.h>

namespace hgl
{
    #define hgl_thread_result unsigned long __stdcall

    #include"../ThreadFunc.h"

    /**
    * (线程外部调用)执行当前线程，线程优先级为tplevel
    * @param tplevel 线程优先级
    * @return 是否创建线程成功
    */
    bool Thread::Start()
    {
        unsigned long threadid;

        threadptr=::CreateThread(0,0,ThreadFunc,this,0,&threadid);

        if(!threadptr)
        {
            LOG_ERROR(OS_TEXT("创建线程失败，Windows错误码：")+OSString((uint)GetLastError()));
            return(false);
        }

        return(true);
    }

    /**
    * (线程外部调用)关闭当前线程.不推荐使用此函数，正在执行的线程被强制关闭会引起无法预知的错误。
    */
    void Thread::Close()
    {
        if(!threadptr)return;

        TerminateThread(threadptr,0);
        CloseHandle(threadptr);
        threadptr=nullptr;
    }

    /**
    * 是否是当前线程
    */
    bool Thread::IsCurThread()
    {
        return(threadptr==GetCurrentThread());
    }

    /**
    * (线程外部调用)等待当前线程
    * @param time_out 等待的时间，如果为0表示等到线程运行结束为止。默认为0
    */
    void Thread::Wait(double time_out)
    {
        if(threadptr)
            WaitForSingleObject(threadptr,time_out>0?time_out*1000:INFINITE);
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
            obj[i]=mt[i]->threadptr;

        WaitForMultipleObjects(count,obj,false,time_out>0?time_out*1000:INFINITE);

        delete[] obj;
    }
}//namespace hgl
