#ifndef HGL_GRAPH_GEN_FRAGMENT_SHADER_INCLUDE
#define HGL_GRAPH_GEN_FRAGMENT_SHADER_INCLUDE

#include"ShaderStringList.h"
#include<hgl/graph/Material.h>
#include<hgl/graph/PixelCompoment.h>
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

                LightMode light_mode;
                bool sun_light;

				bool color_material;

				uint32 mtc_tex_type[mtcMax];
                uint32 tex_pf[mtcMax];              //纹理象素格式
				UTF8String tex_sampler_rgb[mtcMax];
                UTF8String tex_sampler_alpha[mtcMax];
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

				void set_light_mode(const LightMode &lm){light_mode=lm;}
                void add_sun_light(){sun_light=true;}

			public:		//贴图

				void add_in_texture(int,uint,uint,const char *source);
				void add_in_texcoord(int,const char *);

			public:		//最终计算

				bool add_end();				//普通深度计算

				//void add_gbuffer_encode();
				//bool add_ds_end();			//延迟渲染首次计算
			};//struct fs
		}//namespace shadergen
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_GEN_FRAGMENT_SHADER_INCLUDE
