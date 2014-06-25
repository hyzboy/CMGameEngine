#include<hgl/affect/Thrust.h>
#include<hgl/affect/Timer.h>

namespace hgl
{
	namespace affect
	{
		/**
		* 2D推力影响器构造函数
		* @param t 时间记录器
		* @param v 推力方向
		* @param ss 起始力量
		* @param ds 每秒力量衰减幅度
		*/
		Thrust2D::Thrust2D(Timer *t,const Vertex2f &v,double ss,double ds)
		{
			time=t;

			power=ss;
			dec_power=ds;

			dir=v;

			hglSetProperty(Power,		this,Thrust2D::GetPower,	Thrust2D::SetPower);
			hglSetProperty(DecPower,	this,Thrust2D::GetDecPower,	Thrust2D::SetDecPower);
			hglSetProperty(Direction,	this,Thrust2D::GetDir,		Thrust2D::SetDir);

			hglSetPropertyRead(DirectionPower,this,Thrust2D::GetDirPower);
		}

		void Thrust2D::Update()
		{
			if(!time)return;
			if(power<=0)return;

			double gap=time->LastGapTime;

			dir_power=dir*(power*gap);			//计算当前时间所取得的推力

			power-=power*(dec_power*gap);		//力量衰减
		}
	}//namespace affect
}//namespace hgl
