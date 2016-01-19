#ifndef HGL_GRAPH_GEN_FRAGMENT_SHADER_INCLUDE
#define HGL_GRAPH_GEN_FRAGMENT_SHADER_INCLUDE

#include"ShaderStringList.h"
#include<hgl/graph/Material.h>
#include<hgl/graph/ColorFormat.h>
namespace hgl
{
	namespace graph
	{
		namespace shadergen
		{
			struct fs:public shader_stringlist
			{
				shader_in_type in_normal;
				shader_in_type in_color;

				bool color_material;
				
				int mtc[mtcMax];
				UTF8String tex_sampler[mtcMax];
				UTF8String tex_coord[mtcMax];

				bool alpha_test;
				bool outside_discard;

			public:

				fs();

				void add_frag_color();

				void add_alpha_test();
				void open_outside_discard(){outside_discard=true;}

			public:		//法线

				void add_in_normal();

			public:		//颜色

				void set_color_material();

				void add_in_color();

			public:		//贴图

				void add_in_texture(int,int,const char *source);
				void add_in_texcoord(int,const char *);

			public:		//最终计算

				bool add_end();
			};//struct fs
		}//namespace shadergen
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_GEN_FRAGMENT_SHADER_INCLUDE
