#include<hgl/thread/CondVar.h>
#include<apr_thread_cond.h>
#include<hgl/thread/ThreadMutex.h>

namespace hgl
{
	void GetWaitTime(struct timespec &abstime,const double t);

	class CondVarAPR:public CondVar
	{
		apr_thread_cond_t *cond;

	public:

		CondVarAPR(){apr_thread_cond_create(&cond,get_default_apr_pool());}
		~CondVarAPR(){apr_thread_cond_destroy(cond);}

		bool Wait(ThreadMutex *tm,double t)
		{
			if(t>0)
			{
				struct timespec abstime;

				GetWaitTime(abstime,t);

				return apr_thread_cond_timedwait(cond,(apr_thread_mutex_t *)(tm->GetThreadMutex()),(apr_interval_time_t)(time*1000))==APR_SUCCESS;
			}
			else
			{
				return apr_thread_cond_wait(cond,(apr_thread_mutex_t *)(tm->GetThreadMutex()))==APR_SUCCESS;
			}
		}

		void Signal()
		{
			apr_thread_cond_signal(cond);
		}

		void Broadcast()
		{
			apr_thread_cond_broadcast(cond);
		}
	};//class CondVarAPR

	CondVar *CreateCondVarAPR()
	{
		return(new CondVarAPR());
	}
}//namespace hgl
