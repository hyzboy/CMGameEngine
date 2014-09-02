#include<hgl/Other.h>
#if HGL_OS != HGL_OS_Windows
#include<sys/time.h>
#else
#include<minwinbase.h>

namespace hgl
{
	namespace
	{
#define APR_DELTA_EPOCH_IN_USEC 11644473600000000		//转Windows时间到Unix时间的一个差值
		//Windows是1601.1.1
		//Unix是1970.1.1
		//tanks for APR

		static inline void FileTimeToMicroTime(uint64 *result, FILETIME *input)
		{
			/* Convert FILETIME one 64 bit number so we can work with it. */
			*result = input->dwHighDateTime;
			*result = (*result) << 32;
			*result |= input->dwLowDateTime;
			*result /= 10;    /* Convert from 100 nano-sec periods to micro-seconds. */
			*result -= APR_DELTA_EPOCH_IN_USEC;  /* Convert from Windows epoch to Unix epoch */
			return;
		}
	}//namespace
}//namespace hgl
#endif//HGL_OS != HGL_OS_Windows

namespace hgl
{
	namespace
	{
		uint64 program_start_time=0;

		long gmt_off=0;			//windows下为long

		struct TimeInit
		{
			TimeInit()
			{
				program_start_time=GetMicroTime();

			#if HGL_OS == HGL_OS_Windows
				_tzset();
				_get_timezone(&gmt_off);
			#else
				tzset();
				gmt_off=-timezone;
			#endif//HGL_OS == HGL_OS_Windows
			}
		};//struct TimeInit

		static TimeInit time_init;
	}//namespace

	/**
	 * 返回时区时差
	 */
	int GetTimeZone()
	{
		return gmt_off;
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
		return ((tv.tv_sec) * 1000) + (tv.tv_usec/1000);
#else
		return(GetMicroTime() / 1000);
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
		return (tv.tv_sec) * HGL_MICRO_SEC_PER_SEC + tv.tv_usec;
#else
		SYSTEMTIME st;
		FILETIME ft;
		uint64 result;

		GetLocalTime(&st);

		SystemTimeToFileTime(&st, &ft);

		FileTimeToMicroTime(&result, &ft);

		return(result);
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
		return(double(GetMicroTime()) / HGL_MICRO_SEC_PER_SEC);
#endif//HGL_OS != HGL_OS_Windows
	}

	/**
	* 等待指定时间
	* @param time 时间(单位：秒)
	*/
	void WaitTime(double t)
	{
		if(t<=0)return;

#if HGL_OS == HGL_OS_Windows
		::Sleep(DWORD(t*HGL_MILLI_SEC_PRE_SEC));
#else
	#if HGL_OS == HGL_OS_OS2
		DosSleep(t*HGL_MILLI_SEC_PRE_SEC);
	#elif (HGL_OS == HGL_OS_BeOS)||(HGL_OS == HGL_OS_Haiku)
		snooze(t*HGL_MICRO_SEC_PER_SEC);
	#elif HGL_OS == HGL_OS_NetWare
		delay(t*HGL_MILLI_SEC_PRE_SEC);
	#else
		struct timeval tv;
		tv.tv_sec = t;
		tv.tv_usec = (t-tv.tv_sec)*HGL_MICRO_SEC_PER_SEC;
		select(0, NULL, NULL, NULL, &tv);
	#endif
#endif//HGL_OS == HGL_OS_Windows
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
