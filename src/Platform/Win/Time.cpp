#include<hgl/platform/Platform.h>
#include<hgl/TypeFunc.h>

namespace hgl
{
    namespace
    {
#define APR_DELTA_EPOCH_IN_USEC 11644473600000000        //转Windows时间到Unix时间的一个差值
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

    long GetGMTOff()
    {
        SYSTEMTIME st;
        FILETIME ft;
        uint64 local_time;
        uint64 utc_time;

        GetLocalTime(&st);
        SystemTimeToFileTime(&st, &ft);
        FileTimeToMicroTime(&local_time, &ft);

        GetSystemTime(&st);
        SystemTimeToFileTime(&st, &ft);
        FileTimeToMicroTime(&utc_time, &ft);

        return(utc_time - local_time);
    }

    /**
    * 取得当前时间
    * @return 当前时间(单位：百万分之一秒)
    */
    uint64 GetMicroTime()
    {
        SYSTEMTIME st;
        FILETIME ft;
        uint64 result;

        GetLocalTime(&st);

        SystemTimeToFileTime(&st, &ft);

        FileTimeToMicroTime(&result, &ft);

        return(result);
    }
    
    /**
    * 取得当前时间
    * @return 当前时间(单位：千分之一秒)
    */
    uint64 GetTime()
    {
        return(GetMicroTime() / 1000);
    }

    /**
    * 取得当前时间(双精度)
    * @return 当前时间(单位：秒)
    */
    double GetDoubleTime()                                                                        ///<取得当前时间(双精度，单位秒)
    {
        return(double(GetMicroTime()) / HGL_MICRO_SEC_PER_SEC);
    }

    /**
    * 等待指定时间
    * @param time 时间(单位：秒)
    */
    void WaitTime(double t)
    {
        if(t<=0)return;

        ::Sleep(DWORD(t*HGL_MILLI_SEC_PRE_SEC));
    }
}//namespace hgl
