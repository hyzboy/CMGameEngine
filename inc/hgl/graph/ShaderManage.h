#ifndef HGL_GRAPH_SHADER_MANAGE_INCLUDE
#define HGL_GRAPH_SHADER_MANAGE_INCLUDE

#include<hgl/type/ResManage.h>
#include<hgl/graph/Shader.h>
#include<hgl/graph/RenderState.h>
namespace hgl
{
	namespace graph
	{
		/**
		 * Shader管理仓库
		 */
		class ShaderManage:public ResManage<RenderState,Shader>
		{
		protected:

			Shader *Create(const RenderState &) override;
		};//class ShaderManage

		extern ShaderManage *global_shader_manage;							///<全局Shader仓库
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_SHADER_MANAGE_INCLUDE
