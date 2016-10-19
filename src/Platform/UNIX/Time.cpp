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

namespace hgl
{
    long GetGMTOff()
    {
        tzset();
        return(-timezone);
    }

	/**
	* 取得当前时间
	* @return 当前时间(单位：千分之一秒)
	*/
	uint64 GetTime()
	{
		struct timeval tv;
		gettimeofday(&tv, nullptr);
		return ((tv.tv_sec) * 1000) + (tv.tv_usec/1000);
	}

	/**
	* 取得当前时间
	* @return 当前时间(单位：百万分之一秒)
	*/
	uint64 GetMicroTime()
	{
		struct timeval tv;
		gettimeofday(&tv, nullptr);
		return (tv.tv_sec) * HGL_MICRO_SEC_PER_SEC + tv.tv_usec;
	}

	/**
	* 取得当前时间(双精度)
	* @return 当前时间(单位：秒)
	*/
	double GetDoubleTime()																		///<取得当前时间(双精度，单位秒)
	{
		struct timeval tv;
		gettimeofday(&tv, nullptr);
		return double(tv.tv_sec) + (double(tv.tv_usec)/HGL_MICRO_SEC_PER_SEC);
	}

	/**
	* 等待指定时间
	* @param time 时间(单位：秒)
	*/
	void WaitTime(double t)
	{
		if(t<=0)return;

		struct timeval tv;
		tv.tv_sec = t;
		tv.tv_usec = (t-tv.tv_sec)*HGL_MICRO_SEC_PER_SEC;
		select(0, NULL, NULL, NULL, &tv);
	}
}//namespace hgl
