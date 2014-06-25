#include<hgl/graph/Shader.h>
#include<hgl/Other.h>

namespace hgl
{
	namespace graph
	{
		ShaderDataBlock::ShaderDataBlock(const char *name,int size)
		{
			hgl::strcpy(shader_name,HGL_MAX_SHADER_NAME_LENGTH,name);

			block_size=size;
		}
	}//namespace graph
}//namespace hgl
