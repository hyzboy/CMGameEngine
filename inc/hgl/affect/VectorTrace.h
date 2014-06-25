#ifndef HGL_AFFECTOR_VECTOR_TRACE_INCLUDE
#define HGL_AFFECTOR_VECTOR_TRACE_INCLUDE

#include<hgl/affect/AffectorObject.h>
#include<hgl/type/Vertex2.h>
namespace hgl
{
	namespace affect
	{
		class Timer;

		/**
		* 矢量跟踪影响器
		*/
		class VectorTrace2D:public AffectorObject													///矢量跟踪影响器
		{
			Timer *time;																			///<时间记录器

			Vertex2f Speed;

			float TraceRate;

		public:	//属性

			Vertex2f Coord;                                                                         ///<当前坐标点
			Vertex2f Target;                                                                        ///<目标点

			float MaxSpeed;                                                                         ///<最大速度(默认20)
			float MinDistance;                                                                      ///<最小距离(默认10)
			float MaxSpeedDistance;																	///<最大速度所需距离(默认1000)
			float DecSpeed;																			///<减速幅度(默认0.75)

		public:	//事件

    		DefEvent(void,OnToEnd,(VectorTrace2D *));												///<到达目的地事件

		public:

			VectorTrace2D(Timer *,const Vertex2f &coord,const Vertex2f &speed);
			virtual ~VectorTrace2D()HGL_DEFAULT_MEMFUNC;

			virtual const char16_t *GetClassName()const{return u"VectorTrace2D";}					///<取得类名

			virtual void Update();                                                                  ///<更新坐标点
		};//class VectorTrace2D
	}//namespace affect
}//namespace hgl
#endif//HGL_AFFECTOR_VECTOR_TRACE_INCLUDE
