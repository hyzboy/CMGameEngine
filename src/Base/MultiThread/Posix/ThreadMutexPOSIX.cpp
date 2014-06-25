#include<hgl/thread/ThreadMutex.h>
#include<hgl/TypeFunc.h>
#include<pthread.h>
#include<sys/time.h>

namespace hgl
{
	void GetWaitTime(struct timespec &,double);

	ThreadMutex::ThreadMutex()
	{
		pthread_mutexattr_t attr;

		pthread_mutexattr_init(&attr);

		pthread_mutex_init(&ptr,&attr);

		pthread_mutexattr_destroy(&attr);
	}

	ThreadMutex::~ThreadMutex()
	{
		pthread_mutex_destroy(&ptr);
	}

	/**
	* 取得控制权
	* 如果对象处于排斥状态，则等待
	*/
	void ThreadMutex::Lock()
	{
		pthread_mutex_lock(&ptr);
	}

	/**
	* 尝试取得控制权
	* @return 是否成功取得控制权
	*/
	bool ThreadMutex::TryLock()
	{
		return(!pthread_mutex_trylock(&ptr));
	}

	/**
	* 等待并取得控制权
	* @param time 等待的最大时间,时间为0表示尝试
	* @return 是否取得控制权
	*/
	bool ThreadMutex::WaitLock(double t)
	{
		struct timespec abstime;

		GetWaitTime(abstime,t);

		return !pthread_mutex_timedlock(&ptr, &abstime);
	}

	/**
	* 放弃控制权
	*/
	void ThreadMutex::Unlock()
	{
		pthread_mutex_unlock(&ptr);
	}
}//namespace hgl
