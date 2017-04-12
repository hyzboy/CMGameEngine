#include<hgl/Console.h>
#include<hgl/Time.h>
#include<hgl/type/DateTime.h>
#include<hgl/type/BaseString.h>
#include<iostream>

using namespace hgl;
using namespace std;

int main(int,char **)
{
    const double cur_time=GetDoubleTime();

    Date d;
    Time t;

    ToDateTime(d,t,cur_time);

    UTF8String str;

    str=cur_time;

    cout<<"cur_time="<<str.c_str()<<endl;
    cout<<"\tyear="<<d.GetYear()<<" month="<<d.GetMonth()<<" day="<<d.GetDay()<<endl;
    cout<<"\thour="<<t.GetHour()<<" minute="<<t.GetMinute()<<" second="<<t.GetSecond()<<" micro_second="<<t.GetMicroSecond()<<endl;
    cout<<endl;

    const double new_time=FromDateTime(d,t);

    str=new_time;
    cout<<"new_time="<<str.c_str()<<endl;

    cout<<"TimeZone:"<<GetTimeZone()<<endl;
    return(0);
}
