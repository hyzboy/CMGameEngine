#include<hgl/thread/CondVar.h>
#include<hgl/thread/ThreadMutex.h>
#include<pthread.h>

namespace hgl
{
	void GetWaitTime(struct timespec &,double);

	CondVar::CondVar()
	{
		cond_var=new pthread_cond_t;

		pthread_cond_init((pthread_cond_t *)cond_var,nullptr);
	}

	CondVar::~CondVar()
	{
		pthread_cond_destroy((pthread_cond_t *)cond_var);
		delete (pthread_cond_t *)cond_var;
	}

	bool CondVar::Wait(ThreadMutex *tm,double t)
	{
		if(t>0)
		{
			struct timespec abstime;

			GetWaitTime(abstime,t);

			return(!pthread_cond_timedwait((pthread_cond_t *)cond_var,tm->GetThreadMutex(),&abstime));
		}
		else
		{
			return(!pthread_cond_wait((pthread_cond_t *)cond_var,tm->GetThreadMutex()));
		}
	}

	void CondVar::Signal()
	{
		pthread_cond_signal((pthread_cond_t *)cond_var);
	}

	void CondVar::Broadcast()
	{
		pthread_cond_broadcast((pthread_cond_t *)cond_var);
	}
}//namespace hgl
