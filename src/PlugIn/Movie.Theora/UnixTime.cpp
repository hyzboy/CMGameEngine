#include<sys/time.h>
#include"TheoraPlayer.h"

double TheoraPlayer::GetDoubleTime()																		///<取得当前时间(双精度，单位秒)
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return double(tv.tv_sec) + (double(tv.tv_usec)/HGL_MICRO_SEC_PER_SEC);
}
