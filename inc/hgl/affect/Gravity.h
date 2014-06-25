#ifndef HGL_AFFECTOR_GRAVITY_INCLUDE
#define HGL_AFFECTOR_GRAVITY_INCLUDE

#include<hgl/affect/AffectorObject.h>
namespace hgl
{
	namespace affect
	{
		class Timer;

		/**
		* 2D重力影响器<br>
		* 一种提供给2D游戏用的重力模拟影响器,注意它并不符合物理学
		*/
		class Gravity2D:public AffectorObject														///2D重力影响器
		{
			Timer *time;																			///<时间计录器

			double speed;																			///<落体速度
			double add_speed;																		///<重力加速度
			double max_speed;																		///<最大加速度

		private:

			double GetSpeed(){return speed;}
			double GetAddSpeed(){return add_speed;}
			double GetMaxSpeed(){return max_speed;}

			void SetSpeed(double s){speed=s;}
			void SetAddSpeed(double as){add_speed=as;}
			void SetMaxSpeed(double ms){max_speed=ms;}

		public:

			Property<double> Speed;																	///<落体速度
			Property<double> AddSpeed;																///<重力加速度
			Property<double> MaxSpeed;																///<最大加速度

		public:

			Gravity2D(Timer *,double ss=0.0f,double as=1.1f,double ms=16.0f);
			virtual ~Gravity2D()HGL_DEFAULT_MEMFUNC;

			virtual const char16_t *GetClassName(){returnu"Gravity2D";}								///<取得类名

			void Update();
		};//class Gravity2D
	}//namespace affect
}//namespace hgl
#endif//HGL_AFFECTOR_GRAVITY_INCLUDE
