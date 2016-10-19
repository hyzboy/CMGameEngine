#include<hgl/TypeFunc.h>
#include<sys/time.h>

namespace hgl
{
	void GetWaitTime(struct timespec &abstime,double t)
	{
		clock_gettime(CLOCK_REALTIME,&abstime);

		abstime.tv_sec+=t;
		t-=(long)t;
		abstime.tv_nsec+=t*HGL_NANO_SEC_PER_SEC;

		if (abstime.tv_nsec >= HGL_NANO_SEC_PER_SEC)
		{
			abstime.tv_nsec -= HGL_NANO_SEC_PER_SEC;
			++abstime.tv_sec;
		}
	}

	void SetTimeVal(timeval &tv,const double t_sec)
	{
		tv.tv_sec=t_sec;
		tv.tv_usec=(t_sec-tv.tv_sec)*HGL_MICRO_SEC_PER_SEC;
	}
}//namespace hgl
