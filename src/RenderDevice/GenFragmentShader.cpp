#include"GenFragmentShader.h"
#include"OpenGLRenderable.h"
#include"GLSL.h"
#include<hgl/graph/Texture.h>
#include<hgl/LogInfo.h>
#include<stdio.h>
#include<string.h>

namespace hgl
{
	namespace graph
	{
		namespace shadergen
		{
			const char shader_get_sampler_color[3][16]=
			{
				"texture1D",
				"texture2D",
				"texture3D"
			};

			const char HGL_FS_DIFFUSE_COLOR[]="DiffuseColor";
			const char HGL_FS_ALPHA[]="Alpha";

			fs::fs():shader_stringlist()
			{
				in_normal=sitNone;
				in_color=sitNone;

				for(int i=0;i<mtcMax;i++)
					mtc[i]=-1;

				alpha_test=false;
				outside_discard=false;
			}

			void fs::add_frag_color()
			{
				add_format("layout(location=0,index=0) out vec4 %s;\n",HGL_FS_FRAG_COLOR);
			}

			void fs::add_alpha_test()
			{
				alpha_test=true;

				add_uniform_float(HGL_FS_ALPHA_TEST);
			}

			void fs::add_in_normal()
			{
				add_in_vec3(HGL_FS_NORMAL);

				in_normal=sitVertexAttrib;
			}

			void fs::add_in_color()
			{
				in_color=sitVertexAttrib;

				add_in_fv(HGL_FS_COLOR,4);
			}

			void fs::add_uniform_color()
			{
				in_color=sitUniform;

				add_uniform_fv(HGL_FS_COLOR,4);
			}

			/**
			* 增加一张贴图
			* @param mtc_index 成份索引
			* @param num 坐标维数
			* @param source 数据源名称
			*/
			void fs::add_in_texture(int mtc_index,int coord_num,const char *source)
			{
				mtc[mtc_index]=coord_num;

				::strcpy_s(tex_coord[mtc_index],SHADER_VALUE_NAME_MAX_LENGTH,HGL_FS_TEXCOORD);
				::strcat_s(tex_coord[mtc_index],SHADER_VALUE_NAME_MAX_LENGTH,source);

				::strcpy_s(tex_sampler[mtc_index],SHADER_VALUE_NAME_MAX_LENGTH,shader_get_sampler_color[coord_num-1]);
				::strcat_s(tex_sampler[mtc_index],SHADER_VALUE_NAME_MAX_LENGTH,"(");
				::strcat_s(tex_sampler[mtc_index],SHADER_VALUE_NAME_MAX_LENGTH,MaterialTextureName[mtc_index]);
				::strcat_s(tex_sampler[mtc_index],SHADER_VALUE_NAME_MAX_LENGTH,",");
				::strcat_s(tex_sampler[mtc_index],SHADER_VALUE_NAME_MAX_LENGTH,HGL_FS_TEXCOORD);
				::strcat_s(tex_sampler[mtc_index],SHADER_VALUE_NAME_MAX_LENGTH,source);
				::strcat_s(tex_sampler[mtc_index],SHADER_VALUE_NAME_MAX_LENGTH,")");

				add_sampler(MaterialTextureName[mtc_index],coord_num);
			}

			/**
			* 增加一个贴图坐标数据
			* @param num 坐标维数
			* @param fragname 贴图名称
			*/
			void fs::add_in_texcoord(int num,const char *fragname)
			{
				char texcoord_name[SHADER_VALUE_NAME_MAX_LENGTH];

				::strcpy_s(texcoord_name,SHADER_VALUE_NAME_MAX_LENGTH,HGL_FS_TEXCOORD);
				::strcat_s(texcoord_name,SHADER_VALUE_NAME_MAX_LENGTH,fragname);

				add_in_fv(texcoord_name,num);
			}

			bool fs::add_end()
			{
				char fin_color[SHADER_VALUE_NAME_MAX_LENGTH];

				if(mtc[mtcDiffuse]!=-1)		//如果漫反射贴图也为否，则完全无色彩信息，退出
				{
					if(outside_discard)		//有出界放弃处理
					{
						add_format(	"\tif(%s.x<0||%s.x>1\n"
									"\t ||%s.y<0||%s.y>1)discard;\n\n",tex_coord[mtcDiffuse],tex_coord[mtcDiffuse],tex_coord[mtcDiffuse],tex_coord[mtcDiffuse]);
					}

//					if(cf==HGL_COLOR_RGBA)
					{
						add_format("\tvec4 %s=%s",	HGL_FS_DIFFUSE_COLOR,tex_sampler[mtcDiffuse]);
					}
					//else if(cf==HGL_COLOR_RGB)
					//{
					//	add_format("\tvec4 %s=vec4(%s(%s,%s).rgb,1.0)",	HGL_FS_DIFFUSE_COLOR,
					//													shader_get_sampler_color[mtc[mtcDiffuse]-1],
					//													MaterialTextureName[mtcDiffuse],
					//													texcoord_name);
					//}

					::strcpy_s(fin_color,SHADER_VALUE_NAME_MAX_LENGTH,HGL_FS_DIFFUSE_COLOR);

					if(in_color)							//还有顶点颜色传入
					{
						add("*");
						add(HGL_FS_COLOR);
					}

					add(";\n\n");
				}
				else	//无漫反射贴图
				{
					if(!in_color)		//也无顶点颜色
						return(false);	//即使有其它贴图，也必须有颜色

					::strcpy_s(fin_color,SHADER_VALUE_NAME_MAX_LENGTH,HGL_FS_COLOR);
				}

				if(mtc[mtcAlpha]!=-1)				//透明贴图
				{
					add_format("\tfloat %s=%s.r;\n",HGL_FS_ALPHA,tex_sampler[mtcAlpha]);

					add();
				}

				if(alpha_test)		//alpha test
				{
					if(mtc[mtcAlpha]!=-1)
						add_format("\tif(%s.a*%s<%s)discard;\n",fin_color,HGL_FS_ALPHA,HGL_FS_ALPHA_TEST);
					else
						add_format("\tif(%s.a<%s)discard;\n",fin_color,HGL_FS_ALPHA_TEST);

					add();
				}

				if(in_normal)		//计算法线
				{
//					add_format("\tfloat normal_instensity=max(dot(%s,vec3(0.0,0.0,1.0)),0.0);\n",HGL_FS_NORMAL);
//					add();

					if(mtc[mtcAlpha]!=-1)
						add_format("\t%s=%s*vec4(vec3(%s),%s);\n",HGL_FS_FRAG_COLOR,fin_color,HGL_FS_LIGHT_INTENSITY,HGL_FS_ALPHA);
					else
						add_format("\t%s=%s*vec4(vec3(%s),1.0);\n",HGL_FS_FRAG_COLOR,fin_color,HGL_FS_LIGHT_INTENSITY);
				}
				else
				{
					if(mtc[mtcAlpha]!=-1)
						add_format("\t%s=vec4(%s.rgb,%s.a*%s);\n",HGL_FS_FRAG_COLOR,fin_color,fin_color,HGL_FS_ALPHA);
					else
						add_format("\t%s=%s;\n",HGL_FS_FRAG_COLOR,fin_color);
				}

				return(true);
			}
		}//namespace shadergen

		/**
		* 生成Fragment Shader代码
		* @param able 可渲染数据指针
		* @return 生成的Fragment Shader代码
		* @return NULL 生成失败
		*/
#ifdef _DEBUG
		char *MakeFragmentShader(Renderable *able,RenderState *state,const u16char *filename)
#else
		char *MakeFragmentShader(Renderable *able,RenderState *state)
#endif//_DEBUG
		{
			if(!able)return(nullptr);

			Material *mat=able->GetMaterial();

			if(!mat)return(nullptr);

			shadergen::fs code;

			code.add_version(330);		//OpenGL 3.3

			//颜色
			{
				if(state->vertex_color)					//使用顶点颜色
				{
					code.add_in_color();
					code.add();
				}
				else
				if(state->color_material)				//使用独立颜色传入
				{
					code.add_uniform_color();			//使用uniform传入颜色
					code.add();
				}
			}

			//灯光
			if(state->lighting)
			{
				code.add_in_float(HGL_FS_LIGHT_INTENSITY);
				code.add();

				//法线
				//法线无论那一种方式传入，都必须在vertex shader中乘上mv_matrix

				if(state->vertex_normal)				//使用顶点法线
				{
					code.add_in_normal();
					code.add();
				}

//				if(state->normal_map)
//				{
//				}
			}

			if(mat->GetTextureNumber())							//如果有贴图
			{
 				int count=0;

				if(state->height_map)							//有高度图的处理方式
				{
					code.add_in_texcoord(2,MaterialTextureName[mtcHeight]);		//从高度图输入各种坐标
				}

				for(int mtc=0;mtc<mtcMax;mtc++)
				{
					if(mtc==mtcHeight)continue;					//高度图不在fs处理

					Texture *tex=mat->GetTexture(mtc);			//取一下测试无指定坐标贴图是否存在

					if(!tex)continue;

					if(state->height_map
					&&(mtc>=mtcDiffuse&&mtc<mtcPalette))		//使用高度图时，这一部分贴图无需贴图坐标
					{
						code.add_in_texture(mtc,tex->GetCoordNumber(),MaterialTextureName[mtcHeight]);			//加入贴图，从高度图顶点得来的纹理坐标
					}
					else
					{
						VertexBufferBase *vb=able->GetTexCoord(mtc);											//普通方式，必须在对应通道有数据,虽然有不需要数据的情况，但我们暂时不处理

						if(!vb)continue;

						code.add_in_texture(mtc,tex->GetCoordNumber(),MaterialTextureName[mtc]);				//从对应通道得来的纹理坐标
						code.add_in_texcoord(vb->GetComponent(),MaterialTextureName[mtc]);
					}

					count++;
				}

				if(count)		//如果没有加贴图，不加这个空行
					code.add();
			}

			//alpha test
			if(state->alpha_test)
			{
				code.add_alpha_test();
				code.add();
			}

			code.add_frag_color();					//指定颜色输出
			code.add();

			code.add_main_begin();

			code.add_end();							//收尾各方面的处理

			code.add_main_end();

#ifdef _DEBUG
			code.debug_out(filename);
#endif//_DEBUG

			return code.get();
		}//char *MakeFragmentShader(Renderable *able)
	}//namespace graph
}//namespace hgl
