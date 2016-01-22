#include"ShaderStringList.h"
#include<malloc.h>
#include<string.h>
#include<stdarg.h>
#include<stdio.h>
#include<hgl/Other.h>
#include<hgl/graph/TextureFormat.h>

namespace hgl
{
	namespace graph
	{
		namespace shadergen
		{
			const char shader_float_type[4][8]=
			{
				"float",
				"vec2",
				"vec3",
				"vec4"
			};

			const char shader_sampler[3][16]=
			{
				"sampler1D",
				"sampler2D",
				"sampler3D",
			};

			const char shader_sampler_shadow[2][16]=
			{
				"sampler1DShadow",
				"sampler2DShadow",
			};

			const char *get_sampler_by_tex_type(uint type)
			{
				switch(type)
				{
					case HGL_TEXTURE_1D:			return "sampler1D";
					case HGL_TEXTURE_2D:			return "sampler2D";
					case HGL_TEXTURE_3D:			return "sampler3D";

					case HGL_TEXTURE_1D_ARRAY:		return "sampler1DArray";
					case HGL_TEXTURE_2D_ARRAY:		return "sampler1DArray";

					case HGL_TEXTURE_RECTANGLE:		return "samplerRect";

					case HGL_TEXTURE_CUBE_MAP:		return "samplerCube";
					case HGL_TEXTURE_CUBE_MAP_ARRAY:return "samplerCubeArray";

					case HGL_TEXTURE_2D_MS:			return "sampler2DMS";
					case HGL_TEXTURE_2D_MS_ARRAY:	return "sampler2DMSArray";

					case HGL_TEXTURE_BUFFER:		return "samplerBuffer";

					default:						return nullptr;
				}
			}

			const char *get_sampler_shadow_by_tex_type(uint type)
			{
				switch(type)
				{
					case HGL_TEXTURE_1D:			return "sampler1DShadow";
					case HGL_TEXTURE_2D:			return "sampler2DShadow";
					case HGL_TEXTURE_RECTANGLE:		return "samplerRectShadow";
					case HGL_TEXTURE_CUBE_MAP:		return "samplerCubeShadow";

					default:						return nullptr;
				}
			}

			void shader_stringlist::debug_out(const os_char *filename)
			{
				if(!filename)return;

				SaveMemoryToFile(filename,shader_string.c_str(),shader_string.Length());
			}

			void shader_stringlist::add_version(int ver)
			{
				add("#version "+UTF8String(ver)+" core\n\n");
			}

			void shader_stringlist::add_main_begin()
			{
				add("void main(void)\n"
					"{\n");
			}

			void shader_stringlist::add_main_end()
			{
				add("}\n");
			}
		}//namespace shadergen
	}//namespace graph
}//namespace hgl
