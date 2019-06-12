#include<hgl/thread/Thread.h>
#include<hgl/thread/ThreadMutex.h>

namespace hgl
{
    /**
     *  tips:   PTHREAD_CREATE_DETACHED 方式创建的线程，在退出时，自动清除线程。无法使用pthread_join函数获取运行状态,pthread_join会返回22号错误
     *         PTHREAD_CREATE_JOINABLE 方式创建的线程，在退出时，不会清除线程，必使使用pthread_join函数获取。或是在退出时使用pthread_detach(pthread_self())。
     */

    /**
     * 线程具体执行函数，会依操作系统的不同而有所差别
     */
    THREAD_FUNC ThreadFunc(Thread *tc)
    {
        tc->live_lock.Lock();

        if(tc->ProcStartThread())
        {
            while(tc->Execute())
            {
                if(tc->exit_lock.TryLock())
                {
                    tc->exit_lock.Unlock();
                    break;
                }
            }

            tc->ProcEndThread();
        }

        if(tc->IsExitDelete())
        {
            tc->live_lock.Unlock();

            HGL_THREAD_DETACH_SELF
            delete tc;
        }
        else
            tc->live_lock.Unlock();

        return(0);
    }
}//namespace hgl
