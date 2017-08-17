#ifndef HGL_GRAPH_GEN_GEOMETRY_SHADER_INCLUDE
#define HGL_GRAPH_GEN_GEOMETRY_SHADER_INCLUDE

#include"ShaderStringList.h"
namespace hgl
{
	namespace graph
	{
		namespace shadergen
		{
			struct gs:public shader_stringlist
			{
			private:

				uint in_prim;
				uint out_prim;
				uint max_vertices;

			public:

				gs(uint,uint,uint);

			public:

				void add_main_end();
			};//struct gs:public shader_stringlist
		}//namespace shadergen
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_GEN_GEOMETRY_SHADER_INCLUDE
