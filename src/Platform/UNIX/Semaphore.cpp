#include<hgl/thread/Semaphore.h>
#include<hgl/LogInfo.h>
#include<pthread.h>
#include<semaphore.h>

namespace hgl
{
	void GetWaitTime(struct timespec &,double);

	/**
	* @param max_count 最大计数
	*/
	Semaphore::Semaphore(int max_count)
	{
		ptr=new sem_t;

		if(sem_init((sem_t *)ptr,PTHREAD_PROCESS_PRIVATE,max_count))
		{
			LOG_ERROR(OS_TEXT("sem_init error,max_count=")+OSString(max_count));
			delete (sem_t *)ptr;
			ptr=nullptr;
		}
	}

	Semaphore::~Semaphore()
	{
		if(!ptr)return;

		sem_destroy((sem_t *)ptr);
		delete (sem_t *)ptr;
	}

	/**
	* 释放一个信号
	* @param n 释放信号数量
	* @return 是否释放成功
	*/
	bool Semaphore::Release(int n)
	{
		if(!ptr)return(false);
		if(n<=0)return(false);

		int result=0;

 		for(int i=0;i<n;i++)
			result+=sem_post((sem_t *)ptr);

		return !result;

		//if(n==1)
		//	return !sem_post((sem_t *)ptr);
		//else
		//	return !sem_post_multiple((sem_t *)ptr,n);	//注：这个函数不是所有os都支持
	}

	/**
	* 尝试获取一个信号
	* @return 是否有取得信号
	*/
	bool Semaphore::TryAcquire()
	{
		if(!ptr)return(false);

		return !sem_trywait((sem_t *)ptr);
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
			return !sem_wait((sem_t *)ptr);
		else
		{
			struct timespec abstime;

			GetWaitTime(abstime,t);

			return !sem_timedwait((sem_t *)ptr,&abstime);
		}
	}
}//namespace hgl
