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
}//namespace hgl

namespace hgl
{
    /**
     * get current timezone to GMT time offset
     * @return time offset (second)
     */
    long GetGMTOff()
    {
        tzset();
        //return(-timezone);   // linux ok,but BSD don't support
        
        time_t rawtime;
        struct tm *timeinfo;
        
        time(&rawtime);
        timeinfo=localtime(&rawtime);
        
        return timeinfo->tm_gmtoff;
    }

	/**
	* 取得当前时间
	* @return 当前时间(单位：千分之一秒)
	*/
	uint64 GetTime()
	{
// 		struct timeval tv;
// 		gettimeofday(&tv, nullptr);
// 		return ((tv.tv_sec) * HGL_MILLI_SEC_PRE_SEC) + (tv.tv_usec/1000);

        struct timespec ts;
        clock_gettime(CLOCK_REALTIME,&ts);
        return (ts.tv_sec*HGL_MILLI_SEC_PRE_SEC)+(ts.tv_nsec/1000000);
	}

	/**
	* 取得当前时间
	* @return 当前时间(单位：百万分之一秒)
	*/
	uint64 GetMicroTime()
	{
//		struct timeval tv;
//		gettimeofday(&tv, nullptr);
//		return (tv.tv_sec) * HGL_MICRO_SEC_PER_SEC + tv.tv_usec;

        struct timespec ts;
        clock_gettime(CLOCK_REALTIME,&ts);
        return (ts.tv_sec*HGL_MICRO_SEC_PER_SEC)+(ts.tv_nsec/1000);
	}

	/**
	* 取得当前时间(双精度)
	* @return 当前时间(单位：秒)
	*/
	double GetDoubleTime()																		///<取得当前时间(双精度，单位秒)
	{
// 		struct timeval tv;
// 		gettimeofday(&tv, nullptr);
// 		return double(tv.tv_sec) + (double(tv.tv_usec)/HGL_MICRO_SEC_PER_SEC);

        struct timespec ts;
        clock_gettime(CLOCK_REALTIME,&ts);
        return double(ts.tv_sec)+double(ts.tv_nsec)/HGL_NANO_SEC_PER_SEC;
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
		select(0, nullptr, nullptr, nullptr, &tv);

        //注：不要使用sleep/nanosleep/clock_nanosleep来代替select，因为它们是真正的暂停执行线程。而sleep是挂起等待，可以被信号打断
	}
}//namespace hgl
