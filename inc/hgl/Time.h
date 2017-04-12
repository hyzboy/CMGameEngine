#ifndef HGL_TIME_INCLUDE
#define HGL_TIME_INCLUDE

#include<hgl/type/DataType.h>

namespace hgl   //时间
{
    int GetTimeZone();																				///<返回时区的时差(单位：秒)

    uint64 GetMilliStartTime();																		///<取得毫秒程序启动时间(单位：1/1000秒)
    uint64 GetMicroStartTime();																		///<取得微秒程序启动时间(单位：1/1000000秒)
    double GetDoubleStartTime();																	///<取得秒程序启动时间(单位：秒)

    uint64 GetTime();																				///<取得当前时间(单位：1/1000秒)
    uint64 GetMicroTime();																			///<取得当前时间(单位：1/1000000秒)
    double GetDoubleTime();																			///<取得当前时间(单位：秒)

    double GetLocalDoubleTime();																	///<取得本地当前时间(单位：秒)

    void WaitTime(double);																			///<等待一定时间(单位：秒)
}//namespace hgl

#endif//HGL_TIME_INCLUDE
