#ifndef HGL_GRAPH_LIGHT_INCLUDE
#define HGL_GRAPH_LIGHT_INCLUDE

#include<hgl/type/Color3f.h>
#include<hgl/type/Color4f.h>
#include<hgl/algorithm/VectorMath.h>
namespace hgl
{
	namespace graph
	{
		/**
		* 光源类型
		*/
		enum LightType
		{
			ltNone=0,			///<起始定义，无意义

            ltSunLight,         ///<太阳光

			ltPoint,			///<点光源
			ltDirection,		///<方向光
			ltSpot,				///<聚光灯
			ltInfiniteSpotLight,///<无尽聚光灯

			ltAreaQuad,			///<四边形面光源

			ltEnd				///<结束定义，无意义
		};//enum LightType

		/**
		* 灯光
		*/
		struct Light
		{
			LightType	type;			///<光源类型

			bool		enabled;		///<是否开启

			Color4f		ambient;		///<环境光
			Color4f		specular;		///<镜面光
			Color4f		diffuse;		///<漫反射
		};//struct Light

        /**
        * 太阳光
        */
        struct SunLight :public Light
        {
            Vector3f direction;
        };

		/**
		 * 方向光
		 */
		struct DirectionLight:public Light
		{
			Vector3f	direction;				///<方向

			float		nDotVP;					///<normal . light direction
			float		nDotHV;					///<normal . half vector
		};//struct DirectionLight

		/**
		* 点光源
		*/
		struct PointLight:public Light
		{
			Vector3f	position;				///<光源位置

			Vector3f	attenuation;			///<constant,linear,quadratic
		};//struct PointLight

		/**
		 * 聚光灯
		 */
		struct SpotLight:public Light
		{
			Vector3f	position;				///<位置
			Vector3f	direction;				///<方向
			Vector3f	attenuation;			///<constant,linear,quadratic

			float		coscutoff;
			float		exponent;
		};//struct SpotLight

		/**
		 * 无尽聚光灯
		 */
		struct InfiniteSpotLight:public Light
		{
			Vector3f	position;
			Vector3f	direction;
			Vector3f	exponent;
		};//struct InfiniteSpotLight
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_LIGHT_INCLUDE
