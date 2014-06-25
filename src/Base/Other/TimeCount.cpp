#include<hgl/Other.h>
#include<apr_time.h>

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
		return apr_time_as_msec(apr_time_now());
	}

	/**
	* 取得当前时间
	* @return 当前时间(单位：百万分之一秒)
	*/
	uint64 GetMicroTime()
	{
		return apr_time_now();
	}

	/**
	* 取得当前时间(双精度)
	* @return 当前时间(单位：秒)
	*/
	double GetDoubleTime()																		///<取得当前时间(双精度，单位秒)
	{
		return double(apr_time_now())/HGL_MICRO_SEC_PER_SEC;
	}

	/**
	* 等待指定时间
	* @param time 时间(单位：秒)
	*/
	void WaitTime(double time)
	{
		if(time<=0)return;

		apr_sleep(time*HGL_MICRO_SEC_PER_SEC);
	}

	void InitStartTime()
	{
		program_start_time=apr_time_now();
	}

	uint64 GetStartTime()
	{
		return apr_time_as_msec(program_start_time);
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
