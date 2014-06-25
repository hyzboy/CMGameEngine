#ifndef HGL_AFFECTOR_THRUST_INCLUDE
#define HGL_AFFECTOR_THRUST_INCLUDE

#include<hgl/affect/AffectorObject.h>
#include<hgl/type/Vertex2.h>
namespace hgl
{
	namespace affect
	{
		class Timer;

		/**
		* 2D推力影响器<br>
		* 一种提供给2D游戏用的推力模拟影响器，注意它并不符合物理学。
		*/
		class Thrust2D:public AffectorObject														///2D推力影响器
		{
			Timer *time;																			///<时间记录器

			double power;																			///<力量
			double dec_power;																		///<力量衰减幅度

			Vertex2f dir;																			///<力量方向

			Vertex2f dir_power;																		///<方向推力(结果)

		private:

			double GetPower(){return power;}
			double GetDecPower(){return dec_power;}
			Vertex2f GetDir(){return dir;}
			Vertex2f GetDirPower(){return dir_power;}

			void SetPower(double p){power=p;}
			void SetDecPower(double p){dec_power=p;}
			void SetDir(Vertex2f v){dir=v;}

		public:

			Property<double> Power;																	///<力量
			Property<double> DecPower;																///<力量衰减幅度(每秒)
			Property<Vertex2f> Direction;															///<推力方向

			Property<Vertex2f> DirectionPower;														///<方向推力(结果,只读)

		public:

			Thrust2D(Timer *,const Vertex2f &v=Vertex2f(0,0),double ss=1.0f,double ds=0.1f);		///<本类构造函数
			virtual ~Thrust2D()HGL_DEFAULT_MEMFUNC;

			virtual const char16_t *GetClassName(){returnu"Thrust2D";}								///<取得类名

			void Update();
		};//class Thrust2D
	}//namespace affect
}//namespace hgl
#endif//HGL_AFFECTOR_THRUST_INCLUDE
