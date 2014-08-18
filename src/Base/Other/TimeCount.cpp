#include<hgl/Other.h>
#if HGL_OS != HGL_OS_Windows
#include<sys/time.h>
#endif//HGL_OS != HGL_OS_Windows

namespace hgl
{
	namespace
	{
		uint64 program_start_time=0;
	}

	/**
	* 取得当前时间
	* @return 当前时间(单位：千分之一秒)
	*/
	uint64 GetTime()
	{
#if HGL_OS != HGL_OS_Windows
		struct timeval tv;
		gettimeofday(&tv, nullptr);
		return (tv.tv_sec * 1000) + (tv.tv_usec/1000);
#else
#endif//HGL_OS != HGL_OS_Windows
	}

	/**
	* 取得当前时间
	* @return 当前时间(单位：百万分之一秒)
	*/
	uint64 GetMicroTime()
	{
#if HGL_OS != HGL_OS_Windows
		struct timeval tv;
		gettimeofday(&tv, nullptr);
		return tv.tv_sec * HGL_MICRO_SEC_PER_SEC + tv.tv_usec;
#else
#endif//HGL_OS != HGL_OS_Windows
	}

	/**
	* 取得当前时间(双精度)
	* @return 当前时间(单位：秒)
	*/
	double GetDoubleTime()																		///<取得当前时间(双精度，单位秒)
	{
#if HGL_OS != HGL_OS_Windows
		struct timeval tv;
		gettimeofday(&tv, nullptr);
		return double(tv.tv_sec) + (double(tv.tv_usec)/HGL_MICRO_SEC_PER_SEC);
#else
#endif//HGL_OS != HGL_OS_Windows
	}

	/**
	* 等待指定时间
	* @param time 时间(单位：秒)
	*/
	void WaitTime(double t)
	{
		if(time<=0)return;

#if HGL_OS == HGL_OS_Windows
#else
	#if HGL_OS == HGL_OS_OS2
		DosSleep(t/1000);
	#elif (HGL_OS == HGL_OS_BeOS)||(HGL_OS == HGL_OS_Haiku)
		snooze(t);
	#elif HGL_OS == HGL_OS_NetWare
		delay(t/1000);
	#else
		struct timeval tv;
		tv.tv_sec = t;
		tv.tv_usec = (t-tv.tv_sec)*HGL_MICRO_SEC_PER_SEC;
		select(0, NULL, NULL, NULL, &tv);
	#endif
#endif//HGL_OS == HGL_OS_Windows
	}

	void InitStartTime()
	{
		program_start_time=GetMicroTime();
	}

	uint64 GetMilliStartTime()
	{
		return program_start_time/1000;
	}

	uint64 GetMicroStartTime()
	{
		return program_start_time;
	}

	double GetDoubleStartTime()
	{
		return double(program_start_time)/HGL_MICRO_SEC_PER_SEC;
	}
}//namespace hgl
