#ifndef HGL_GRAPH_GEN_VERTEX_SHADER_INCLUDE
#define HGL_GRAPH_GEN_VERTEX_SHADER_INCLUDE

#include"ShaderStringList.h"
#include<hgl/graph/Material.h>
#include<hgl/graph/ColorFormat.h>
namespace hgl
{
	namespace graph
	{
		namespace shadergen
		{
			struct vs:public shader_stringlist
			{
				shader_in_type in_normal;
				shader_in_type in_vertex_color;

				bool color_material;

				UTF8String vertex_color_to_vec4;

				int vertex_type;

				bool mvp_matrix;

				bool in_vertex_buffer[HGL_MAX_VERTEX_BUFFER];

				int in_texcoord_count;
				int in_texcoord_num[HGL_MAX_TEX_COORD];

				int out_texcoord_count;
				bool out_texcoord[HGL_MAX_TEX_COORD];
				UTF8String out_texcoord_name[HGL_MAX_TEX_COORD];
				UTF8String out_texcoord_source[HGL_MAX_TEX_COORD];		//来源

				int in_tex_count;
				bool in_tex[mtcMax];
				UTF8String tex_sampler[mtcMax];

				Axis height_axis;				//高度图所用轴

            protected:

                void add_layout_in(int vb,const char *name,int num);

			protected:

				int SetVertexColorFormat(ColorFormat);

			public:

				vs();

			public:

				void add_mvp(bool);

			protected:	//顶点

				int vertex_format;

			public:		//顶点

				void add_in_vertex(int);

			public:		//法线

				void add_in_normal();
				void add_uniform_normal();

			public:		//颜色

				void set_color_material();

				void add_in_color(ColorFormat fmt);

			public:		//纹理

				void add_in_texture(const char *,int,uint,int mtc);
				void add_in_texcoord(int,const char *,int,int mtc);
				void add_out_texcoord(int,int mtc,const char *);

			public:		//高度图

				void set_height_axis(Axis a){height_axis=a;}

			public:		//最终计算

				bool add_end();
			};//struct vs
		}//namespace shadergen
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_GEN_VERTEX_SHADER_INCLUDE
