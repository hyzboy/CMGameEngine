#ifndef HGL_GRAPH_DEFERRED_SHADING_INCLUDE
#define HGL_GRAPH_DEFERRED_SHADING_INCLUDE

#include"../GLSL/GLSL.h"
namespace hgl
{
	namespace graph
	{
		namespace shadergen
		{
			//延迟渲染最终合成shader

			const char deferred_shading_vs[]=	"#version 330 core\n"
												"\n"
												"in vec3 " HGL_VS_VERTEX ";\n"
												"in vec2 " HGL_VS_TEXCOORD_GBUFFER ";\n"
												"\n"
												"out vec2 " HGL_FS_TEXCOORD_GBUFFER ";\n"
												"\n"
												"void main()\n"
												"{\n"
												"\tgl_Position = vec4(" HGL_VS_VERTEX ", 1.0f);\n"
												"\t" HGL_FS_TEXCOORD_GBUFFER "= " HGL_VS_TEXCOORD_GBUFFER ";\n"
												"}";

			const char deferred_shading_fs[]=	"#version 330 core\n"
												"\n"
												"uniform sampler
												"\n"
												"in vec2 " HGL_FS_TEXCOORD_GBUFFER ";\n"

												"}";
		}//namespace shadergen
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_DEFERRED_SHADING_INCLUDE
