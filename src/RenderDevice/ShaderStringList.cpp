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

			shader_stringlist::shader_stringlist()
			{
				str=(char *)hgl_malloc(1024);

				length=0;
				max_length=1023;
			}

			shader_stringlist::~shader_stringlist()
			{
			}

			void shader_stringlist::add(const char *line)
			{
				int add_length=::strlen(line);

				if(length+add_length>=max_length)
				{
					max_length=((length+add_length+1+1023)/1024)*1024;

					str=(char *)hgl_realloc(str,max_length);
				}

				memcpy(str+length,line,add_length+1);		//+1是为了把0也复制进去

				length+=add_length;
			}

			void shader_stringlist::add_format(const char *fmt,...)
			{
				va_list va;

				char result[1024];

				va_start(va,fmt);

	#ifdef _MSC_VER
				vsnprintf_s(result,1024,1024,fmt,va);
	#else
				vsnprintf(result,1024,fmt,va);
	#endif//_MSC_VER

				va_end(va);

				add(result);
			}

			void shader_stringlist::debug_out(const os_char *filename)
			{
				if(!filename)return;

				SaveMemoryToFile(filename,str,length);
			}

			void shader_stringlist::add_version(int ver)
			{
				add_format("#version %d\n",ver);
				add();
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
