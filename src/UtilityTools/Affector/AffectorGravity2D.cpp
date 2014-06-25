#include<hgl/affect/Gravity.h>
#include<hgl/affect/Timer.h>

namespace hgl
{
	namespace affect
	{
		/**
		* 本类构造函数
		* @param t 计时器
		* @param ss 起始速度
		* @param as 加速度
		* @param ms 最大速度
		*/
		Gravity2D::Gravity2D(Timer *t,double ss,double as,double ms)
		{
			time=t;

			speed=ss;
			add_speed=as;
			max_speed=ms;

			hglSetProperty(Speed,	this,Gravity2D::GetSpeed,	Gravity2D::SetSpeed);
			hglSetProperty(AddSpeed,this,Gravity2D::GetAddSpeed,Gravity2D::SetAddSpeed);
			hglSetProperty(MaxSpeed,this,Gravity2D::GetMaxSpeed,Gravity2D::SetMaxSpeed);
		}

		void Gravity2D::Update()
		{
			if(!time)return;

			speed+=add_speed*time->LastGapTime;

			if(speed>max_speed)		//达到最快速度了,这个不是追踪，不用减速
				speed=max_speed;
		}
	}//namespace affect
}//namespace hgl
