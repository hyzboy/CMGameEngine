#include<hgl/Other.h>

namespace hgl
{
    long GetGMTOff();

    namespace
    {
        uint64 program_start_time=0;

        long gmt_off=0;            //windows下为long

        struct TimeInit
        {
            TimeInit()
            {
                program_start_time=GetMicroTime();

                gmt_off=GetGMTOff();
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
    * 取得本地当前时间(双精度)
    * @return 当前时间(单位：秒)
    */
    double GetLocalDoubleTime()
    {
        return GetDoubleTime()+GetTimeZone();
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
