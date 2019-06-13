#include<windows.h>
#include"TheoraPlayer.h"

double TheoraPlayer::GetDoubleTime()                                                                        ///<取得当前时间(双精度，单位秒)
{
    unsigned __int64 time;

    GetSystemTimeAsFileTime((FILETIME *)&time);

    if(startTime==0)
    {
        startTime=time;
        return(0);
    }
    else
    {
        return double(time-startTime)/10000000.f;
    }
}
