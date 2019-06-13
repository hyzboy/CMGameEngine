#include<hgl/thread/Semaphore.h>
#include<hgl/LogInfo.h>
#include<pthread.h>
#include<dispatch/dispatch.h>

namespace hgl
{
    void GetWaitTime(struct timespec &,double);

    /**
    * @param max_count 最大计数
    */
    Semaphore::Semaphore(int max_count)
    {
        ptr=dispatch_semaphore_create(0);

        if(!ptr)
        {
            LOG_ERROR(OS_TEXT("dispatch_semaphore_create error"));
            ptr=nullptr;
        }
    }

    Semaphore::~Semaphore()
    {
        if(!ptr)return;

        dispatch_release(ptr);
    }

    /**
    * 发送信号
    * @param n 发送的信号数量
    * @return 是否释放成功
    */
    bool Semaphore::Post(int n)
    {
        if(!ptr)return(false);
        if(n<=0)return(false);

        for(int i=0;i<n;i++)
            dispatch_semaphore_signal(ptr);

        return(true);
    }

    /**
    * 尝试获取一个信号
    * @return 是否有取得信号
    */
    bool Semaphore::TryAcquire()
    {
        if(!ptr)return(false);

        return !dispatch_semaphore_wait(ptr, DISPATCH_TIME_NOW);
    }

    /**
    * 等待并获取一个信号
    * @param t 等待的最长时间,使用0表示无限等待.(单位秒)
    * @return 是否等待到了,如果超过最长时间,仍未等到即为超时,返回false
    */
    bool Semaphore::Acquire(double t)
    {
        if(!ptr)return(false);

        if(t<=0)
        {
            return !dispatch_semaphore_wait(ptr, DISPATCH_TIME_FOREVER);
        }
        else
        {
            dispatch_time_t when=dispatch_time(DISPATCH_TIME_NOW,t*HGL_NANO_SEC_PER_SEC);

            return !dispatch_semaphore_wait(ptr,when);
        }
    }
}//namespace hgl
