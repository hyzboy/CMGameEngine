#include<hgl/graph/SceneGlobalState.h>

namespace hgl
{
	namespace graph
	{
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
	}//namespace graph
}//namespace hgl
