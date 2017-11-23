#include<hgl/thread/Thread.h>
#include<hgl/thread/CondVar.h>
#include<hgl/LogInfo.h>
#include<pthread.h>
#include<signal.h>
#include<errno.h>

namespace hgl
{
    /**
      *  tips:	PTHREAD_CREATE_DETACHED 方式创建的线程，在退出时，自动清除线程。无法使用pthread_join函数获取运行状态,pthread_join会返回22号错误
      *         PTHREAD_CREATE_JOINABLE 方式创建的线程，在退出时，不会清除线程，必使使用pthread_join函数获取。或是在退出时使用pthread_detach(pthread_self())。
      */

	void *ThreadFunc(Thread *tc)
	{
        tc->live_lock.Lock();

		if(tc->ProcStartThread())
		{
			while(tc->Execute())
            {
                if(tc->exit_lock.TryLock())
                    break;
            }

			tc->ProcEndThread();
		}

		tc->exit_lock.Unlock();
        tc->live_lock.Unlock();

		if(tc->IsExitDelete())
		{
			delete tc;
			pthread_detach(pthread_self());
		}

		return(0);
	}

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

		if(pthread_create(&tp,&attr,(void *(*)(void *))ThreadFunc,this))		//返回0表示正常
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
	* (线程外部调用)关闭当前线程.不推荐使用此函数，正在执行的线程被强制关闭会引起无法预知的错误。
	*/
	void Thread::ForceClose()
    {
        if(!tp)
        {
            LOG_ERROR(OS_TEXT("Thread::Cancel() error,tp=nullptr."));
            return(true);
        }

        if(pthread_cancel(tp)!=0)		// 0 is success
            return(false);

        tp=0;
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

		return pthread_equal(pthread_self(),tp);		//返回非0表示一致
	}

	void Thread::SetCancelState(bool enable,bool async)
	{
		if(enable)
		{
			pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,nullptr);

			pthread_setcanceltype(async?PTHREAD_CANCEL_ASYNCHRONOUS:			//立即触发
										PTHREAD_CANCEL_DEFERRED					//延后
										,nullptr);
		}
		else
		{
			pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,nullptr);
		}
	}

	void GetWaitTime(struct timespec &abstime,double t);

	/**
	* (线程外部调用)等待当前线程
	* @param time_out 等待的时间，如果为0表示等到线程运行结束为止。默认为0
	*/
	void Thread::Wait(const double time_out)
	{
		if(!tp)
		{
			LOG_ERROR(OS_TEXT("Thread::Wait() error,tp=nullptr."));
			return;
		}

        if(!IsLive())       //已经死了
        {
            tp=0;
            return;
        }

		int retval;
        void *res;

        if(time_out>0)
        {
            struct timespec ts;

            GetWaitTime(ts,time_out);

            retval=pthread_timedjoin_np(tp,&res,&ts);
        }
        else
        {
            retval=pthread_join(tp,&res);
        }

#ifdef _DEBUG
        UTF8String thread_addr;

        GetAddress(thread_addr);

        LOG_INFO(U8_TEXT("pthread_timedjoin_np/pthread_join [")+thread_addr+U8_TEXT("] retval:")+UTF8String(retval));
#endif//_DEBUG

        tp=0;
	}
}//namespace hgl
