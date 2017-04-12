#ifndef HGL_DATETIME_INCLUDE
#define HGL_DATETIME_INCLUDE

#include<hgl/type/DataType.h>
#include<hgl/TypeFunc.h>

namespace hgl
{
	namespace io
	{
		class DataInputStream;
		class DataOutputStream;
	}//namespace io

	/**
	* 时间类，这个类用来保存和计算时间。
	*
	* 当您对Hours,Minutes,Seconds中任何一个值进行加减时，其它值都会自动计算。如：11:30这个值，使用Minutes+=55。会自动计算出55分钟后的时间，值为12:25
	*/
	class Time																											///时间类
	{
		int32 gmt_off;		///<当前时区与UTC时间的差值

		int8 hours;
		int8 minutes;
		int8 seconds;
		int32 micro_seconds;
		int8 week_day;		///<为了取这个星期过了多少秒用的

	public:

		int GetGMT			()const{return gmt_off;}																	///<取得当前时间与UTC时间的时差

		int GetHour			()const{return hours;}																		///<时
		int GetMinute		()const{return minutes;}																	///<分
		int GetSecond		()const{return seconds;}																	///<秒
		int GetMicroSecond	()const{return micro_seconds;}																///<微秒(百万分之一秒)

		int GetDaySeconds	()const{return (hours*HGL_TIME_ONE_HOUR)+(minutes*HGL_TIME_ONE_MINUTE)+seconds;}			///<今天过了多少秒

		int GetWeekSeconds	()const																						///<这个星期过了多少秒
		{
			int week = week_day;
			if(week_day > 0)
				--week;

			if(week_day == 0)
				week=(HGL_DAY_ONE_WEEK-1);

			return ((week*HGL_HOUR_ONE_DAY+hours)*HGL_TIME_ONE_HOUR)+(minutes*HGL_TIME_ONE_MINUTE)+seconds;
		}

// 		int GetPastWeekSeconds()const{return int(GetDoubleTime()-GetWeekSeconds());}									///<过去那些年到这周一0点过了多少秒

		void SetGMT(int go){gmt_off=go;}
		void SetHour(int);
		void SetMinute(int);
		void SetSecond(int);
		void SetMicroSecond(int);

	public:

		Time();
		Time(const double);
		Time(int,int,int,int);
		Time(const Time &);

		void Clear()
		{
			gmt_off=0;
			hours=0;
			minutes=0;
			seconds=0;
			micro_seconds=0;
			week_day=0;
		}

		void Set(int h,int m=0,int s=0,int ms=0,int wd=0);

		Time &operator = (const Time &);

		const int Comp(const Time &)const;

		CompOperator(const Time &,Comp);

		void Sync(const double=0);																	///<和系统时间同步

		bool SaveToStream(io::DataOutputStream *) const;
		bool LoadFromStream(io::DataInputStream *);
	};//class Time

	/**
	* 日期类，这个类用来保存和计算日期。带有闰年的处理
	*
	* 当您对Year,Month,Day中任意一个值进行修改时，其它值都会自动跟着计算。如1981-4-17,如果使用Day+=400，会自动计算出400天之后的日期，结果是1982-5-21
	*/
	class Date																						///日期类
	{
		int32 year;
		int8 month;
		int8 day;

		int8 week_day;
		int16 year_day;

		int8 max_days_per_month;

	public:

		int GetYear		()const{return year;}														///<年
		int GetMonth	()const{return month;}														///<月
		int GetDay		()const{return day;}														///<日
		int GetMaxDays	()const{return max_days_per_month;}													///<本月天数
		int GetWeekDay	()const{return week_day;}													///<星期,0为星期天
		int GetYearDay	()const{return year_day;}													///<今天为这一年的第几天

		void SetYear	(int y){year=y;}
		void SetMonth	(int);
		void SetDay		(int);

	public:

		Date();
		Date(const double);
		Date(int,int,int);
		Date(const Date &);

		void Clear()
		{
			year=0;
			month=0;
			day=0;

			week_day=0;
			year_day=0;

			max_days_per_month=0;
		}

		void Set(int y,int m,int d,int wd=0,int yd=0);

		Date &operator = (const Date &);

		const int Comp(const Date &)const;

		CompOperator(const Date &,Comp);

		void Sync(const double=0);																	///<和系统日期同步

		bool SaveToStream(io::DataOutputStream *) const;
		bool LoadFromStream(io::DataInputStream *);
	};//class Date

	/**
	 * 根据当前时间，转换出Date/Time两个结构
	 */
	void ToDateTime(Date &,Time &,const double cur_time=0);

	/**
	 * 根据日期和时间，转换出以秒为单位的时间值
	 * @param year 年
	 * @param month 月
	 * @param day 日
	 * @param hour 小时
	 * @param minute 分
	 * @param second 秒
	 * @param micro_second 微秒(1/1000秒)
	 * @return 转换出的值
	 */
	double FromDateTime(const int year,const int month,const int day,
						const int hour,const int minute=0,const int second=0,const int micro_second=0,const int gmt_off=0);

	/**
	 * 根据Date/Time转换出以秒为单位的时间值
	 */
	double FromDateTime(const Date &,const Time &);
}//namespace hgl
#endif//HGL_DATETIME_INCLUDE
