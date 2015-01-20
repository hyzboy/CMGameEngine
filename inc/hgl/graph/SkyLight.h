#ifndef HGL_GRAPH_SKY_LIGHT_INCLUDE
#define HGL_GRAPH_SKY_LIGHT_INCLUDE

#include<hgl/type/Color3f.h>
#include<hgl/VectorMath.h>
namespace hgl
{
	namespace scene
	{
		/**
		* 天空灯光(太阳光/月光)<br>
		* 太阳光/月光相对来讲效果类似于平行激光能，不会衰弱，没有散射。对任何点射入角度一致
		*/
		struct SkyLight
		{
			Color3f ambient;				///<环境光颜色
			Color3f diffuse;				///<漫反射颜色
			Vector4f direction;				///<方向

			float nDotVP;					///<normal . light direction

		public:

			virtual void FromTime(double);	///<根据时间产生天空灯光属性
		};//struct SkyLight
	}//namespace scene
}//namespace hgl
#endif//HGL_GRAPH_SKY_LIGHT_INCLUDE
