#include<hgl/thread/Thread.h>
#include<hgl/thread/CondVar.h>
#include<hgl/LogInfo.h>
#include<pthread.h>
#include<signal.h>
#include<errno.h>

namespace hgl
{
	#define hgl_thread_result void *

	#include"../ThreadFunc.h"

	/**
	* (线程外部调用)执行当前线程
	* @return 是否创建线程成功
	*/
	bool Thread::Start()
	{
		if(tp)
		{
			LOG_ERROR(OS_TEXT("Thread::Start() error,tp!=nullptr."));
			Cancel();
		}

		pthread_attr_t attr;

		pthread_attr_init(&attr);

		if(IsExitDelete())
			pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);              //注意这种方式初始化的，pthread_join会返回22无效
		else
			pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

		if(pthread_create(&tp,&attr,ThreadFunc,this))		//返回0表示正常
		{
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
	void Thread::Close()
	{
		Cancel();
	}

	const uint64 Thread::GetThreadID()const
	{
		return tp;
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

	/**
	 * 放弃当前线程
	 */
	bool Thread::Cancel()																		///<放弃这个线
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

	///**
	//* 强制当前线程放弃处理器
	//*/
	//void Thread::Yield()
	//{
	//	apr_thread_yield();
	//}

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

		int retval;
        void *res;

        retval=pthread_kill(tp,0);           //这个函数不是用来杀线程的哦，而是用来向线程发送一个关闭的信号。而发送0代表获取这个线程是否还活着

        if(retval!=ESRCH                            //线程不存在
         &&retval!=EINVAL)                          //信号不合法
        {
#ifdef _DEBUG
            char str[(sizeof(void *)+1)<<1];
            UTF8String thread_addr;

            htos(str,(sizeof(void *)+1)<<1,(uint64)tp);

            thread_addr=str;

            LOG_INFO(U8_TEXT("pthread_kill [")+thread_addr+U8_TEXT("] retval:")+UTF8String(retval));
#endif//_DEBUG

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

            //如果retval返回22，请检查线程初始化时，是否为PTHREAD_CREATE_DETACHED

#ifdef _DEBUG
            LOG_INFO(U8_TEXT("pthread_timedjoin_np/pthread_join [")+thread_addr+U8_TEXT("] retval:")+UTF8String(retval));
#endif//_DEBUG
        }

        tp=0;
	}
}//namespace hgl
