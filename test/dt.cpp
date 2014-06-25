#include<hgl/Console.h>
#include<hgl/type/DateTime.h>
#include<iostream>

using namespace hgl;
using namespace std;

HGL_CONSOLE_MAIN_FUNC()
{
	const double cur_time=GetDoubleTime();

	Date d;
	Time t;

	ToDateTime(d,t,cur_time);

	cout<<"cur_time="<<(uint)cur_time<<endl;
	cout<<"\tyear="<<d.GetYear()<<" month="<<d.GetMonth()<<" day="<<d.GetDay()<<endl;
	cout<<"\thour="<<t.GetHour()<<" minute="<<t.GetMinute()<<" second="<<t.GetSecond()<<" micro_second="<<t.GetMicroSecond()<<endl;
	cout<<endl;

	const double new_time=FromDateTime(d,t);

	cout<<"new_time="<<(uint)new_time<<endl;
}
