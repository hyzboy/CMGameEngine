#ifndef HGL_GRAPH_SHADER_STORAGE_INCLUDE
#define HGL_GRAPH_SHADER_STORAGE_INCLUDE

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
		class ShaderStorage:public ResManage<RenderState,Shader>
		{
		protected:

			virtual Shader *Create(const RenderState &);						///<资源创建虚拟函数无实现，请特例化实现
			virtual void Clear(Shader *obj){delete obj;}						///<资源释放虚拟函数(缺省为直接delete对象)
		};//class ShaderStorage
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_SHADER_STORAGE_INCLUDE
