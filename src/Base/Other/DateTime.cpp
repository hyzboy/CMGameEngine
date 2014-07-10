#include <hgl/type/DataType.h>
#include <hgl/type/DateTime.h>
#include <apr_time.h>

#include <hgl/io/DataInputStream.h>
#include <hgl/io/DataOutputStream.h>

#include<hgl/thread/RWLock.h>

namespace hgl
{
	bool Time::SaveToStream(io::DataOutputStream *str) const
	{
		if(!str)return(false);

		if(!str->WriteInt8(hours))return(false);
		if(!str->WriteInt8(minutes))return(false);
		if(!str->WriteInt8(seconds))return(false);
		if(!str->WriteInt32(micro_seconds))return(false);
		if(!str->WriteInt32(gmt_off))return(false);

		return(true);
	}

	bool Time::LoadFromStream(io::DataInputStream *str)
	{
		if(!str)return(false);

		if(!str->ReadInt8(hours))return(false);
		if(!str->ReadInt8(minutes))return(false);
		if(!str->ReadInt8(seconds))return(false);
		if(!str->ReadInt32(micro_seconds))return(false);
		if(!str->ReadInt32(gmt_off))return(false);

		return(true);
	}

	Time::Time()
	{
		hours=0;
		minutes=0;
		seconds=0;
		micro_seconds=0;
	}

	Time::Time(const double t)
	{
		Sync(t);
	}

	Time::Time(int h,int m,int s,int ms)
	{
		hours=h;
		minutes=m;
		seconds=s;
		micro_seconds=ms;
	}

	Time::Time(const Time &t)
	{
		operator = (t);
	}

	/**
	 * 设置时间
	 * @param h 小时
	 * @param m 分
	 * @param s 秒
	 * @param ms 微秒(百分分之一秒)
	 */
	void Time::Set(int h,int m,int s,int ms,int wd)
	{
		hours=h;
		minutes=m;
		seconds=s;
		micro_seconds=ms;
		week_day=wd;
	}

	Time &Time::operator=(const Time &t)
	{
		hours			=t.hours;
		minutes			=t.minutes;
		seconds			=t.seconds;
		micro_seconds	=t.micro_seconds;

		return(*this);
	}

	const int Time::Comp(const Time &t)const
	{
		if(hours!=t.hours)
			return hours-t.hours;

		if(minutes!=t.minutes)
			return minutes-t.minutes;

		if(seconds!=t.seconds)
			return seconds-t.seconds;

		return micro_seconds-t.micro_seconds;
	}

	void Time::SetHour(int h)
	{
		if(h<0)
		{
			while(h<0)
				h+=HGL_HOUR_ONE_DAY;
		}
		else if(h>0)
		{
			while(h>(HGL_HOUR_ONE_DAY-1))
				h-=HGL_HOUR_ONE_DAY;
		}

		hours=h;
	}

	void Time::SetMinute(int m)
	{
		int h=hours;

		if(m<0)
		{
			while(m<0)
			{
				m+=HGL_TIME_ONE_MINUTE;
				h--;
			}
		}
		else if(m>0)
		{
			while(m>(HGL_TIME_ONE_MINUTE-1))
			{
				m-=HGL_TIME_ONE_MINUTE;
				h++;
			}
		}

		minutes=m;
		SetHour(h);
	}

	void Time::SetSecond(int s)
	{
		int m=minutes;

		if(s<0)
		{
			while(s<0)
			{
				s+=HGL_TIME_ONE_MINUTE;
				m--;
			}
		}
		else if(s>0)
		{
			while(s>(HGL_TIME_ONE_MINUTE-1))
			{
				s-=HGL_TIME_ONE_MINUTE;
				m++;
			}
		}

		seconds=s;
		SetMinute(m);
	}

	void Time::SetMicroSecond(int ms)
	{
		int s=seconds;

		if(ms<0)
		{
			while(ms<0)
			{
				ms-=HGL_MICRO_SEC_PER_SEC;
				s--;
			}
		}
		else if(ms>0)
		{
			while(ms>(HGL_MICRO_SEC_PER_SEC-1))
			{
				ms-=HGL_MICRO_SEC_PER_SEC;
				s++;
			}
		}

		micro_seconds=ms;
		SetSecond(s);
	}

	//和系统时间同步
	void Time::Sync(const double t)
	{
		apr_time_exp_t d;

		if(t<=0)
			apr_time_exp_lt(&d,apr_time_now());
		else
			apr_time_exp_lt(&d,t*HGL_MICRO_SEC_PER_SEC);

		hours			=d.tm_hour;
		minutes			=d.tm_min;
		seconds			=d.tm_sec;
		micro_seconds	=d.tm_usec;
		week_day		=d.tm_wday;

		gmt_off			=d.tm_gmtoff;
	}
}//namespace hgl
//--------------------------------------------------------------------------------------------------
namespace hgl
{
	Date::Date(const double t)
	{
		Sync(t);
	}

	Date::Date(const Date &t)
	{
		operator = (t);
	}

	Date &Date::operator=(const Date &t)
	{
		year	=t.year;
		month	=t.month;
		day		=t.day;
		max_days_per_month=t.max_days_per_month;
		week_day=t.week_day;
		year_day=t.year_day;

		return(*this);
	}

	const int Date::Comp(const Date &t)const
	{
		if(year!=t.year)
			return year-t.year;

		if(month!=t.month)
			return month-t.month;

		return day-t.day;
	}

	bool Date::SaveToStream(io::DataOutputStream *str) const
	{
		if(!str)return(false);

		if(!str->WriteInt32(year))return(false);
		if(!str->WriteInt8(month))return(false);
		if(!str->WriteInt8(day))return(false);
		if(!str->WriteInt8(max_days_per_month))return(false);
		if(!str->WriteInt8(week_day))return(false);
		if(!str->WriteInt16(year_day))return(false);

		return(true);
	}

	bool Date::LoadFromStream(io::DataInputStream *str)
	{
		if(!str)return(false);

		if(!str->ReadInt32(year))return(false);
		if(!str->ReadInt8(month))return(false);
		if(!str->ReadInt8(day))return(false);
		if(!str->ReadInt8(max_days_per_month))return(false);
		if(!str->ReadInt8(week_day))return(false);
		if(!str->ReadInt16(year_day))return(false);

		return(true);
	}

	Date::Date()
	{
		year=2000;
		month=1;
		day=1;
		week_day=0;
		year_day=0;
		max_days_per_month=0;
	}

	Date::Date(int y,int m,int d)
	{
		Set(y,m,d);
	}

	void Date::Set(int y,int m,int d,int wd,int yd)
	{
		day=d;
		month=m;
		year=y;
		week_day=wd;
		year_day=yd;
		max_days_per_month=0;
	}

	void Date::SetMonth(int m)
	{
		if(m<1)
		{
			while(m<1)
			{
				m+=HGL_MONTH_ONE_YEAR;
				year--;
			}
		}
		else
		{
			while(m>HGL_MONTH_ONE_YEAR)
			{
				m-=HGL_MONTH_ONE_YEAR;
				year++;
			}
		}

		month=m;

		const int md[HGL_MONTH_ONE_YEAR]={31,0,31,30,31,30,31,31,30,31,30,31};

		if(month==2)
			max_days_per_month=((year%4)?28:29);
		else
			max_days_per_month=md[month-1];
	}

	void Date::SetDay(int d)
	{
		int m=month;

		if(d<1)
		{
			while(d<1)
			{
				d+=max_days_per_month;
				m--;
			}
		}
		else
		{
			while(d>max_days_per_month)
			{
				d-=max_days_per_month;
				m++;
			}
		}

		day=d;
		SetMonth(m);
	}

	void Date::Sync(const double t)
	{
		apr_time_exp_t d;

		if(t<=0)
			apr_time_exp_lt(&d,apr_time_now());
		else
			apr_time_exp_lt(&d,t*HGL_MICRO_SEC_PER_SEC);

		year	=d.tm_year+1900;
		SetMonth(d.tm_mon+1);
		SetDay	(d.tm_mday);
		week_day=d.tm_wday;
		year_day=d.tm_yday;
	}
}//namespace hgl

namespace hgl
{
	void ToDateTime(Date &d,Time &t,const double cur_time)
	{
		apr_time_exp_t at;

		if(cur_time<=0)
			apr_time_exp_lt(&at,apr_time_now());
		else
			apr_time_exp_lt(&at,cur_time*HGL_MICRO_SEC_PER_SEC);

		d.Set(at.tm_year+1900,at.tm_mon+1,at.tm_mday,at.tm_wday,at.tm_yday);
		t.Set(at.tm_hour,at.tm_min,at.tm_sec,at.tm_usec,at.tm_wday);
		t.SetGMT(at.tm_gmtoff);
	}

	double FromDateTime(const int year,const int month,const int day,
						const int hour,const int minute,const int second,const int micro_second,
						const int gmt_off
   					)
	{
		apr_time_exp_t at;

		hgl_zero(at);

		at.tm_year	=year-1900;
		at.tm_mon	=month-1;
		at.tm_mday	=day;

		at.tm_hour	=hour;
		at.tm_min	=minute;
		at.tm_sec	=second;
		at.tm_usec	=micro_second;

		at.tm_gmtoff=gmt_off;

		apr_time_t result;

		apr_time_exp_gmt_get(&result,&at);

		return double(result)/HGL_MICRO_SEC_PER_SEC;
	}

	double FromDateTime(const Date &d,const Time &t)
	{
		return FromDateTime(d.GetYear(),d.GetMonth(),d.GetDay(),
							t.GetHour(),t.GetMinute(),t.GetSecond(),t.GetMicroSecond(),
							t.GetGMT());
	}
}//namespace hgl
