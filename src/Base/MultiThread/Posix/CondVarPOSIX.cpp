#include<hgl/thread/CondVar.h>
#include<hgl/thread/ThreadMutex.h>
#include<pthread.h>
#include<errno.h>
#include<iostream>

namespace hgl
{
	void GetWaitTime(struct timespec &,double);

	class CondVarPOSIX:public CondVar
	{
		pthread_cond_t *cond;

	public:

		CondVarPOSIX(pthread_cond_t *c)
		{
			cond=c;
		}

		~CondVarPOSIX()
		{
			pthread_cond_destroy(cond);
			delete cond;
		}

		bool Wait(ThreadMutex *tm,double t)
		{
			if(t>0)
			{
				struct timespec abstime;

				GetWaitTime(abstime,t);

				return(!pthread_cond_timedwait(cond,tm->GetThreadMutex(),&abstime));
			}
			else
			{
				return(!pthread_cond_wait(cond,tm->GetThreadMutex()));
			}
		}

		void Signal()
		{
			pthread_cond_signal(cond);
		}

		void Broadcast()
		{
			pthread_cond_broadcast(cond);
		}
	};//class CondVarAPR

	CondVar *CreateCondVarPOSIX()
	{
		pthread_cond_t *cond=new pthread_cond_t;

		int rv=pthread_cond_init(cond,nullptr);

		if(rv)
		{
			std::cout<<"pthread_cond_init error,errno:"<<errno<<std::endl;
			delete cond;
			return(nullptr);
		}

		return(new CondVarPOSIX(cond));
	}
}//namespace hgl
