#ifndef HGL_AFFECTOR_GRAVITATION_INCLUDE
#define HGL_AFFECTOR_GRAVITATION_INCLUDE

#include<hgl/affect/AffectorObject.h>
namespace hgl
{
	namespace affect
	{
		class Timer;

		/**
		* 重力影响器<br>
		* 一种提供给游戏用的重力模拟影响器,注意它并不符合物理学
		*/
		template<typename T>
		class Gravitation:public AffectorObject														///重力影响器
		{
			Timer *time;																			///<时间计录器

			float speed;																			///<落体速度
			float add_speed;																		///<重力加速度
			float max_speed;																		///<最大加速度

		private:

			float GetSpeed(){return speed;}
			float GetAddSpeed(){return add_speed;}
			float GetMaxSpeed(){return max_speed;}

			void SetSpeed(float s){speed=s;}
			void SetAddSpeed(float as){add_speed=as;}
			void SetMaxSpeed(float ms){max_speed=ms;}

		public:

			Property<float> Speed;																	///<落体速度
			Property<float> AddSpeed;																///<重力加速度
			Property<float> MaxSpeed;																///<最大加速度

		public:

			/**
			* 本类构造函数
			* @param t 计时器
			* @param ss 起始速度
			* @param as 加速度
			* @param ms 最大速度
			*/
			Gravitation(Timer *t,double ss=0.0f,double as=1.1f,double ms=16.0f)
			{
				time=t;

				speed=ss;
				add_speed=as;
				max_speed=ms;

				hglSetProperty(Speed,	this,Gravitation::GetSpeed,		Gravitation::SetSpeed	);
				hglSetProperty(AddSpeed,this,Gravitation::GetAddSpeed,	Gravitation::SetAddSpeed);
				hglSetProperty(MaxSpeed,this,Gravitation::GetMaxSpeed,	Gravitation::SetMaxSpeed);
			}

			virtual ~Gravitation()HGL_DEFAULT_MEMFUNC;

			virtual const u16char *GetClassName(){return u"Gravitation";}							///<取得类名

			virtual void Update()
			{
				if(!time)return;

				speed+=add_speed*time->LastGapTime;

				if(speed>max_speed)		//达到最快速度了,这个不是追踪，不用减速
					speed=max_speed;
			}
		};//class Gravitation

		using Gravitation2f=Gravitation<Vector2f>;
		using Gravitation3f=Gravitation<Vector3f>;
	}//namespace affect
}//namespace hgl
#endif//HGL_AFFECTOR_GRAVITATION_INCLUDE
