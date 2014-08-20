#include<hgl/type/DataType.h>
#include<hgl/type/DateTime.h>

#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>

#include<hgl/thread/RWLock.h>

#include<time.h>

#if HGL_OS==HGL_OS_Windows
namespace hgl
{
	namespace
	{
#define APR_DELTA_EPOCH_IN_USEC 11644473600000000		//转Windows时间到Unix时间的一个差值
		//Windows是1601.1.1
		//Unix是1970.1.1
		//tanks for APR

		static inline void SystemTimeToMicroTime(uint64 *result, SYSTEMTIME *st_input)
		{
			FILETIME ft;

			SystemTimeToFileTime(st_input, &ft);

			/* Convert FILETIME one 64 bit number so we can work with it. */
			*result = ft.dwHighDateTime;
			*result = (*result) << 32;
			*result |= ft.dwLowDateTime;
			*result /= 10;    /* Convert from 100 nano-sec periods to micro-seconds. */
			*result -= APR_DELTA_EPOCH_IN_USEC;  /* Convert from Windows epoch to Unix epoch */
			return;
		}

		static inline void MicroTimeToFileTime(LPFILETIME pft, uint64 t)
		{
			LONGLONG ll;
			t += APR_DELTA_EPOCH_IN_USEC;
			ll = t * 10;
			pft->dwLowDateTime = (DWORD)ll;
			pft->dwHighDateTime = (DWORD)(ll >> 32);
			return;
		}

		static inline void MicroTimeToSystemTime(LPSYSTEMTIME pst, uint64 t)
		{
			FILETIME ft;

			MicroTimeToFileTime(&ft, t);

			FileTimeToSystemTime(&ft, pst);
		}
	}
}//namespace hgl
#else
	#include<sys/time.h>
#endif//HGL_OS==HGL_OS_Windows

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
	void Time::Sync(const double cur_time)
	{
#if HGL_OS != HGL_OS_Windows
		tm m;
		time_t tt;

		if(cur_time<=0)
			tt=time(nullptr);
		else
			tt=cur_time*HGL_MICRO_SEC_PER_SEC;

		localtime_r(&tt,&m);

		hours			=m.tm_hour;
		minutes			=m.tm_min;
		seconds			=m.tm_sec;
		micro_seconds	=tt%HGL_MICRO_SEC_PER_SEC;
		week_day		=m.tm_wday;

		gmt_off			=m.tm_gmtoff;
#else
		uint64 st64;
		uint64 lt64;

		SYSTEMTIME st;
		SYSTEMTIME local_time;
		TIME_ZONE_INFORMATION time_zone;

		if (cur_time <= 0)
		{
			GetSystemTime(&st);
			SystemTimeToMicroTime(&st64, &st);
		}
		else
		{
			st64 = cur_time*HGL_MICRO_SEC_PER_SEC;
			MicroTimeToSystemTime(&st, st64);
		}

		GetTimeZoneInformation(&time_zone);

		SystemTimeToTzSpecificLocalTime(&time_zone, &st, &local_time);

		hours			= local_time.wHour;
		minutes			= local_time.wMinute;
		seconds			= local_time.wSecond;
		micro_seconds	= local_time.wMilliseconds * 1000;
		week_day		= local_time.wDayOfWeek;

		SystemTimeToMicroTime(&lt64, &local_time);

		gmt_off			= lt64 - st64;
#endif//HGL_OS != HGL_OS_Windows
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

	void Date::Sync(const double cur_time)
	{
	#if HGL_OS != HGL_OS_Windows
		tm m;
		time_t tt;

		if(cur_time<=0)
			tt=time(nullptr);
		else
			tt=cur_time*HGL_MICRO_SEC_PER_SEC;

		localtime_r(&tt,&m);

		year	=m.tm_year+1900;
		SetMonth(m.tm_mon+1);
		SetDay	(m.tm_mday);
		week_day=m.tm_wday;
		year_day=m.tm_yday;
	#else
		SYSTEMTIME st;

		if (cur_time <= 0)
			GetLocalTime(&st);
		else
			MicroTimeToSystemTime(&st, cur_time*HGL_MICRO_SEC_PER_SEC);

		year = st.wYear;
		month = st.wMonth;
		day = st.wDay;

		week_day = st.wDayOfWeek;
	#endif//HGL_OS != HGL_OS_Windows
	}
}//namespace hgl

namespace hgl
{
	void ToDateTime(Date &d,Time &t,const double cur_time)
	{
#if HGL_OS != HGL_OS_Windows
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
#else
		FILETIME ft,local_ft;
		SYSTEMTIME st;

		MicroTimeToFileTime(&ft, cur_time*HGL_MICRO_SEC_PER_SEC);

		FileTimeToSystemTime(&ft, &st);
		FileTimeToLocalFileTime(&ft, &local_ft);

		d.Set(st.wYear, st.wMonth, st.wDay, st.wDayOfWeek);
		t.Set(st.wHour, st.wMinute, st.wSecond, st.wMilliseconds * 1000, st.wDayOfWeek);
		t.SetGMT((local_ft.dwLowDateTime - ft.dwLowDateTime) / 10);
#endif//HGL_OS != HGL_OS_Windows
	}

	double FromDateTime(const int year,const int month,const int day,
						const int hour,const int minute,const int second,const int micro_second,
						const int gmt_off)
	{
#if HGL_OS != HGL_OS_Windows
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
#else
		SYSTEMTIME st;

		st.wYear = year;
		st.wMonth = month;
		st.wDay = day;
		st.wDayOfWeek = 0;

		st.wHour = hour;
		st.wMinute = minute;
		st.wSecond = second;
		st.wMilliseconds = micro_second / 1000;

		uint64 result;
		SystemTimeToMicroTime(&result, &st);

		result -= gmt_off;
		return(double(result) / HGL_MICRO_SEC_PER_SEC);
#endif//HGL_OS != HGL_OS_Windows
	}

	double FromDateTime(const Date &d,const Time &t)
	{
		return FromDateTime(d.GetYear(),d.GetMonth(),d.GetDay(),
							t.GetHour(),t.GetMinute(),t.GetSecond(),t.GetMicroSecond(),
							t.GetGMT());
	}
}//namespace hgl
