#include<hgl/type/DataType.h>
#include<hgl/type/DateTime.h>

#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>

#include<hgl/thread/RWLock.h>

#include<time.h>
#include<sys/time.h>

namespace hgl
{
	//和系统时间同步
	void Time::Sync(const double cur_time)
	{
		tm m;
		time_t tt;

		if(cur_time<=0)
		{
			struct timeval tv;
			gettimeofday(&tv, nullptr);

			tt=tv.tv_sec;
			micro_seconds=tv.tv_usec;
		}
		else
		{
			tt=cur_time;
			micro_seconds=(cur_time-tt)*HGL_MICRO_SEC_PER_SEC;
		}

		localtime_r(&tt,&m);

		hours			=m.tm_hour;
		minutes			=m.tm_min;
		seconds			=m.tm_sec;
		week_day		=m.tm_wday;

		gmt_off			=m.tm_gmtoff;
	}
}//namespace hgl
//--------------------------------------------------------------------------------------------------
namespace hgl
{
	void Date::Sync(const double cur_time)
	{
		tm m;
		time_t tt;

		if(cur_time<=0)
		{
			struct timeval tv;
			gettimeofday(&tv, nullptr);

			tt=tv.tv_sec;
		}
		else
		{
			tt=cur_time;
		}

		localtime_r(&tt,&m);

		year	=m.tm_year+1900;
		SetMonth(m.tm_mon+1);
		SetDay	(m.tm_mday);
		week_day=m.tm_wday;
		year_day=m.tm_yday;
	}
}//namespace hgl

namespace hgl
{
	void ToDateTime(Date &d,Time &t,const double cur_time)
	{
		tm m;
		time_t tt;
		uint micro_sec;

		if(cur_time<=0)
		{
			struct timeval tv;
			gettimeofday(&tv, nullptr);

			tt=tv.tv_sec;
			micro_sec=tv.tv_usec;
		}
		else
		{
			tt=cur_time;
			micro_sec=(cur_time-tt)*HGL_MICRO_SEC_PER_SEC;
		}

		localtime_r(&tt,&m);

		d.Set(m.tm_year+1900,m.tm_mon+1,m.tm_mday,m.tm_wday,m.tm_yday);
		t.Set(m.tm_hour,m.tm_min,m.tm_sec,micro_sec,m.tm_wday);
		t.SetGMT(m.tm_gmtoff);
	}

	double FromDateTime(const int year,const int month,const int day,
						const int hour,const int minute,const int second,const int micro_second,
						const int gmt_off)
	{
		tm at;

		hgl_zero(at);

		at.tm_year	=year-1900;
		at.tm_mon	=month-1;
		at.tm_mday	=day;

		at.tm_hour	=hour;
		at.tm_min	=minute;
		at.tm_sec	=second;

		at.tm_gmtoff=gmt_off;

		double result=mktime(&at);

		return(result + (double(micro_second) / HGL_MICRO_SEC_PER_SEC));
	}
}//namespace hgl
