#include<hgl/ut/Timer.h>
#include<hgl/Other.h>

namespace hgl
{
	Timer::Timer(double t)
	{
    	hglSetProperty(Interval,this,Timer::GetInterval,Timer::SetInterval);

		if(t)
			Interval=t;
		else
            SetEnabled(false);

		OnTimer=nullptr;
	}

	void Timer::SetInterval(double t)
	{
		inter=t;
		next=GetDoubleTime()+t;
	}

	void Timer::Update()
	{
		if(OnTimer==nullptr)    //没有设置事件函数
            SetEnabled(false);

		double cur_time=GetDoubleTime();

		if(inter&&cur_time>=next)      //间隔大于0，且时间到了
		{
			SafeCallEvent(OnTimer,(this));

			next=cur_time+inter;
		}
	}
}//namespace hgl
