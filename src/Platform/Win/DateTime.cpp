#include<hgl/type/DataType.h>
#include<hgl/type/DateTime.h>

#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>

#include<hgl/thread/RWLock.h>

#include<time.h>

namespace hgl
{
    namespace
    {
#define APR_DELTA_EPOCH_IN_USEC 11644473600000000        //转Windows时间到Unix时间的一个差值
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

namespace hgl
{
    //和系统时间同步
    void Time::Sync(const double cur_time)
    {
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

        hours            = local_time.wHour;
        minutes            = local_time.wMinute;
        seconds            = local_time.wSecond;
        micro_seconds    = local_time.wMilliseconds * 1000;
        week_day        = local_time.wDayOfWeek;

        SystemTimeToMicroTime(&lt64, &local_time);

        gmt_off            = lt64 - st64;
    }
}//namespace hgl
//--------------------------------------------------------------------------------------------------
namespace hgl
{
    void Date::Sync(const double cur_time)
    {
        SYSTEMTIME st;

        if (cur_time <= 0)
            GetLocalTime(&st);
        else
            MicroTimeToSystemTime(&st, cur_time*HGL_MICRO_SEC_PER_SEC);

        year = st.wYear;
        month = st.wMonth;
        day = st.wDay;

        week_day = st.wDayOfWeek;
    }
}//namespace hgl

namespace hgl
{
    void ToDateTime(Date &d,Time &t,const double cur_time)
    {
        FILETIME ft,local_ft;
        SYSTEMTIME st;

        MicroTimeToFileTime(&ft, cur_time*HGL_MICRO_SEC_PER_SEC);

        FileTimeToSystemTime(&ft, &st);
        FileTimeToLocalFileTime(&ft, &local_ft);

        d.Set(st.wYear, st.wMonth, st.wDay, st.wDayOfWeek);
        t.Set(st.wHour, st.wMinute, st.wSecond, st.wMilliseconds * 1000, st.wDayOfWeek);
        t.SetGMT((local_ft.dwLowDateTime - ft.dwLowDateTime) / 10);
    }

    double FromDateTime(const int year,const int month,const int day,
                        const int hour,const int minute,const int second,const int micro_second,
                        const int gmt_off)
    {
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
    }
}//namespace hgl

