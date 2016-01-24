#include<hgl/graph/SceneGlobalState.h>

namespace hgl
{
	namespace graph
	{
		void GetSkyLightColor(Color4f &,uint time);	///<根据时间产生天空灯光属性

		/**
		* 全局场景状态
		*/
		GlobalSceneState::GlobalSceneState()
		{
			lighting=false;

			light_number=0;

			light_list=nullptr;
		}

		GlobalSceneState::~GlobalSceneState()
		{
			SAFE_CLEAR_OBJECT_ARRAY(light_list,light_number);
		}

		/**
		* 设定最大灯光数
		*/
		void GlobalSceneState::SetMaxLight(int max_light)
		{
			if(light_list)
				SAFE_CLEAR_OBJECT_ARRAY(light_list,light_number);

			light_number=max_light;
			light_list=new Light *[light_number];

			for(int i=0;i<light_number;i++)
				light_list[i]=nullptr;
		}

		Light *GlobalSceneState::GetLight(int index)
		{
			if(!light_list
			 ||index<0||index>=light_number)return(nullptr);

			return light_list[index];
		}

		void GlobalSceneState::OpenLight(int index)
		{
			if(!light_list
			 ||index<0||index>=light_number)return;

			if(light_list[index])
				light_list[index]->enabled=true;
		}

		void GlobalSceneState::CloseLight(int index)
		{
			if(!light_list
			 ||index<0||index>=light_number)return;

			if(light_list[index])
				light_list[index]->enabled=false;
		}

		/**
		 * 根据当天当前时间设定天空光照属性
		 * @param day_time 当天的时间，以秒为单位
		 */
		void GlobalSceneState::SetSkyLight(uint day_time)
		{
			GetSkyLightColor(sky_light.ambient,day_time);
		}
	}//namespace graph
}//namespace hgl
