#include"ShaderStringList.h"
#include<malloc.h>
#include<string.h>
#include<stdarg.h>
#include<stdio.h>
#include<hgl/Other.h>

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
