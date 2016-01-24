#ifndef HGL_GRAPH_SCENE_GLOBAL_STATE_INCLUDE
#define HGL_GRAPH_SCENE_GLOBAL_STATE_INCLUDE

#include<hgl/graph/Light.h>
namespace hgl
{
	namespace graph
	{
		/**
		* 场景全局状态
		*/
		class GlobalSceneState
		{
			DirectionLight sky_light;											///<天空光照

			bool lighting;														///<是否开启光照

			int light_number;													///<光源数量

			Light **light_list;													///<光源列表

		public:

			GlobalSceneState();
			virtual ~GlobalSceneState();

			DirectionLight *GetSkyLight	(){return &sky_light;}					///<取得天空灯光
			virtual void	SetSkyLight	(uint);									///<根据当天当前时间设定天空光照属性

			virtual void	SetLighting	(bool l){lighting=l;}					///<设置全局光照
			virtual void	SetMaxLight	(int);									///<设置最大灯光数

			virtual Light *	GetLight	(int);									///<取得指定灯光
			virtual void	OpenLight	(int);									///<开启指定灯光
			virtual void	CloseLight	(int);									///<关闭指定灯光
		};//class GlobalState
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_SCENE_GLOBAL_STATE_INCLUDE
