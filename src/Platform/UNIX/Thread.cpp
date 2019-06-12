#include<hgl/thread/Thread.h>
#include<hgl/thread/CondVar.h>
#include<hgl/LogInfo.h>
#include<signal.h>
#include<errno.h>
#include<hgl/Str.h>

namespace hgl
{
    extern THREAD_FUNC ThreadFunc(Thread *tc);

    /**
    * (线程外部调用)执行当前线程
    * @return 是否创建线程成功
    */
    bool Thread::Start()
    {
        if(tp)
        {
            LOG_ERROR(OS_TEXT("Thread::Start() error,tp!=nullptr."));
            return(false);
        }

        pthread_attr_t attr;

        pthread_attr_init(&attr);

        pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

        exit_lock.Lock();

        if(pthread_create(&tp,&attr,(void *(*)(void *))ThreadFunc,this))        //返回0表示正常
        {
            exit_lock.Unlock();
            tp=0;

            pthread_attr_destroy(&attr);
            LOG_ERROR(OS_TEXT("Create Thread (pthread_create) failed.errno:")+OSString(errno));
            return(false);
        }

        pthread_attr_destroy(&attr);
        return(true);
    }

    /**
    * 是否是当前线程
    */
    bool Thread::IsCurThread()
    {
        if(!tp)
        {
            LOG_ERROR(OS_TEXT("Thread::IsCurThread() error,tp=nullptr."));
            return(true);
        }

        return pthread_equal(pthread_self(),tp);        //返回非0表示一致
    }

    void GetWaitTime(struct timespec &abstime,double t);

    void WaitThreadExit(thread_ptr tp,const double &time_out)
    {
        if(!tp)
        {
            LOG_ERROR(OS_TEXT("WaitThreadExit error,tp=nullptr."));
            return;
        }

        int retval;
        void *res;

#if !defined(__ANDROID__)&&!defined(HGL_OS_BSD)
        if(time_out>0)
        {
            struct timespec ts;

            GetWaitTime(ts,time_out);

            retval=pthread_timedjoin_np(tp,&res,&ts);
        }
        else
#endif//__ANDROID__&&BSD
        {
            retval=pthread_join(tp,&res);
        }

#ifdef _DEBUG

        char thread_addr[(sizeof(thread_ptr)<<1)+1];

        DataToUpperHexStr(thread_addr,(uint8 *)&tp,sizeof(thread_ptr));

        LOG_INFO(U8_TEXT("pthread_timedjoin_np/pthread_join [")+UTF8String(thread_addr)+U8_TEXT("] retval:")+UTF8String(retval));
#endif//_DEBUG
    }
}//namespace hgl
