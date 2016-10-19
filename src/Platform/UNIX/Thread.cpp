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
	* @param tplevel 线程优先级
	* @return 是否创建线程成功
	*/
	bool Thread::Start()
	{
		if(threadptr)
		{
			LOG_ERROR(OS_TEXT("Thread::Start() error,threadptr!=nullptr."));
			Cancel();
		}

		pthread_attr_t attr;

		pthread_attr_init(&attr);

		if(IsExitDelete())
			pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
		else
			pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

		if(pthread_create(&threadptr,&attr,ThreadFunc,this))		//返回0表示正常
		{
			threadptr=0;

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
		return threadptr;
	}

	/**
	* 是否是当前线程
	*/
	bool Thread::IsCurThread()
	{
		if(!threadptr)
		{
			LOG_ERROR(OS_TEXT("Thread::IsCurThread() error,threadptr=nullptr."));
			return(true);
		}

		return pthread_equal(pthread_self(),threadptr);		//返回非0表示一致
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
		if(!threadptr)
		{
			LOG_ERROR(OS_TEXT("Thread::Cancel() error,threadptr=nullptr."));
			return(true);
		}

		if(pthread_cancel(threadptr)!=0)		// 0 is success
			return(false);

		threadptr=0;
		return(true);
	}

	///**
	//* 强制当前线程放弃处理器
	//*/
	//void Thread::Yield()
	//{
	//	apr_thread_yield();
	//}

	/**
	* (线程外部调用)等待当前线程
	* @param time 等待的时间，如果为0表示等到线程运行结束为止。默认为0
	*/
	void Thread::Wait(double time)
	{
		if(!threadptr)
		{
			LOG_ERROR(OS_TEXT("Thread::Wait() error,threadptr=nullptr."));
			return;
		}

		int retval;

        retval=pthread_kill(threadptr,0);

        if(retval==ESRCH
         ||retval==EINVAL)return;

		pthread_join(threadptr,(void **)&retval);
	}
}//namespace hgl
