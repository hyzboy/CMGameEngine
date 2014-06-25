#include"GenVertexShader.h"
#include"OpenGLRenderable.h"
#include"GLSL.h"
#include<hgl/graph/Render.h>
#include<hgl/LogInfo.h>
#include<stdio.h>
#include<string.h>

namespace hgl
{
	namespace graph
	{
		namespace shadergen
		{
			vs::vs():shader_stringlist()
			{
				vertex_format=0;

				mvp_matrix=false;

				in_normal=sitNone;
				in_vertex_color=sitNone;

				vertex_type=0;

				in_tex_count=0;
				in_texcoord_count=0;
				out_texcoord_count=0;

				memset(in_tex				,0,sizeof(in_tex));
				memset(in_texcoord_num		,0,sizeof(in_texcoord_num));
				memset(in_vertex_buffer		,0,sizeof(in_vertex_buffer));
				memset(out_texcoord			,0,sizeof(out_texcoord));
				memset(out_texcoord_name	,0,sizeof(out_texcoord_name));
				memset(out_texcoord_source	,0,sizeof(out_texcoord_source));

				height_axis=HGL_AXIS_NONE;
			}

			void vs::add_mvp(bool normal)
			{
				//add_uniform_mat4(HGL_VS_PROJECTION_MATRIX);
				add_uniform_mat4(HGL_VS_MVP_MATRIX);				//传proj*mv，mv两个而不是proj,mv两个，以省去在vs中计算proj*mv

				if(normal)
					add_uniform_mat4(HGL_VS_NORMAL_MATRIX);

				mvp_matrix=true;
			}

			void vs::add_in_vertex(int num)
			{
				vertex_type=num;

				add_in_fv(HGL_VS_VERTEX,num);

				in_vertex_buffer[vbtVertex]=true;
			}

			void vs::add_in_normal()
			{
				add_in_vec3(HGL_VS_NORMAL);
				//add_out_vec3(HGL_FS_NORMAL);
				add_uniform_float(HGL_VS_GLOBAL_LIGHT_INTENSITY);
				add_out_float(HGL_FS_LIGHT_INTENSITY);

				in_normal=sitVertexAttrib;

				in_vertex_buffer[vbtNormal]=true;
			}

			void vs::add_uniform_normal()
			{
				add_uniform_vec3(HGL_VS_NORMAL);
				add_out_vec3(HGL_FS_NORMAL);

				in_normal=sitUniform;
			}

			int vs::SetVertexColorFormat(ColorFormat fmt)
			{
				if(fmt<=HGL_COLOR_NONE||fmt>=HGL_COLOR_END)return(0);

				if(fmt==HGL_COLOR_ALPHA				){::sprintf(vertex_color_to_vec4,"vec4(1.0,1.0,1.0,%s);",HGL_VS_COLOR);return(1);}
				if(fmt==HGL_COLOR_LUMINANCE			){::sprintf(vertex_color_to_vec4,"vec4(vec3(%s),1.0);",HGL_VS_COLOR);return(1);}
				if(fmt==HGL_COLOR_LUMINANCE_ALPHA	){::sprintf(vertex_color_to_vec4,"vec4(vec3(%s.x),%s.y);",HGL_VS_COLOR,HGL_VS_COLOR);return(2);}
				if(fmt==HGL_COLOR_RGB				){::sprintf(vertex_color_to_vec4,"vec4(%s,1.0);",HGL_VS_COLOR);return(3);}
				if(fmt==HGL_COLOR_RGBA				){::sprintf(vertex_color_to_vec4,"%s;",HGL_VS_COLOR);return(4);}

				LOG_ERROR(OS_TEXT("输入的颜色格式无法处理：")+OSString(fmt));
				return(0);
			}

			void vs::add_in_color(ColorFormat fmt)
			{
				const int num=SetVertexColorFormat(fmt);

				if(num<=0)
				{
					LOG_ERROR(OS_TEXT("顶点色输入无格式信息或格式信息错误"));
					return;
				}

				in_vertex_color=sitVertexAttrib;

				add_in_fv(HGL_VS_COLOR,num);
				add_out_fv(HGL_FS_COLOR,4);

				in_vertex_buffer[vbtColor]=true;
			}

			void vs::add_uniform_color(ColorFormat fmt)
			{
				const int num=SetVertexColorFormat(fmt);

				if(num<=0)
				{
					LOG_ERROR(OS_TEXT("顶点色输入无格式信息或格式信息错误"));
					return;
				}

				in_vertex_color=sitUniform;

				add_uniform_fv(HGL_VS_COLOR,num);
				add_out_fv(HGL_FS_COLOR,4);
			}

			void vs::add_in_texture(const char *vb_name,int coord_num,int mtc_index)
			{
				add_sampler(MaterialTextureName[mtc_index],coord_num);

				::strcpy(tex_sampler[mtc_index],shader_get_sampler_color[coord_num-1]);
				::strcat(tex_sampler[mtc_index],"(");
				::strcat(tex_sampler[mtc_index],MaterialTextureName[mtc_index]);
				::strcat(tex_sampler[mtc_index],",");
				::strcat(tex_sampler[mtc_index],vb_name);
				::strcat(tex_sampler[mtc_index],")");

				in_tex[mtc_index]=true;
				in_tex_count++;
			}

			/**
			* 添加一个纹理坐标输入
			* @param vbt 输入纹理坐标缓冲区编号
			* @param vb 输入纹理坐标缓冲区名称
			* @param coord_num 坐标维数
			* @param mtc_index 目标纹理通道编号
			*/
			void vs::add_in_texcoord(int vbt,const char *vb_name,int coord_num,int mtc_index)
			{
				if(mtc_index<0||mtc_index>=HGL_MAX_TEX_COORD)return;

				in_texcoord_num[mtc_index]=coord_num;

				in_texcoord_count++;

				if(in_vertex_buffer[vbt])return;											//已经定义过了，多个数据共用一个贴图坐标时有可能产生这种情况

				add_in_fv(vb_name,coord_num);												//用vb_name是为了让输入的顶点数据依旧使用TexCoord0,1,2,3这样的名字以便于调试

				in_vertex_buffer[vbt]=true;
			}

			/**
			* 添加一个输出纹理坐标
			* @param coord_num 坐标维数
			* @param mtc_index 目标纹理通道编号
			* @param source 输入数据源，必须额外指名，因为有可能来源并不是当前通道
			*/
			void vs::add_out_texcoord(int coord_num,int mtc_index,const char *source)
			{
				if(mtc_index<0||mtc_index>=HGL_MAX_TEX_COORD)return;

				out_texcoord_count++;
				out_texcoord[mtc_index]=true;

				::strcpy(out_texcoord_name[mtc_index],HGL_FS_TEXCOORD);
				::strcat(out_texcoord_name[mtc_index],MaterialTextureName[mtc_index]);

				::strcpy(out_texcoord_source[mtc_index],source);

				add_out_fv(out_texcoord_name[mtc_index],coord_num);
			}

			bool vs::add_end()
			{
				add("\tvec4 Position;\n");

				if(in_normal)			//有法线要算
				{
					add();
					add("\tvec3 VP;\n");
					add("\tvec3 MVNormal;\n");
				}

				if(in_vertex_color)		//有顶点颜色
				{
					add();
					add_format("\t%s=%s\n",HGL_FS_COLOR,vertex_color_to_vec4);
				}

				if(out_texcoord_count)	//有纹理坐标需要输出到fs
				{
					int count=0;

					for(int i=0;i<HGL_MAX_TEX_COORD;i++)
						if(out_texcoord[i])
							count++;

					if(count)
					{
						add();

						for(int i=0;i<HGL_MAX_TEX_COORD;i++)
						{
							if(!out_texcoord[i])continue;

							add_format("\t%s=%s;\n",out_texcoord_name[i],out_texcoord_source[i]);
						}
					}
				}

				//gl_Position计算
				{
					char pos_str[1024]="\tPosition=";

					if(vertex_type==2)
					{
						::strcat(pos_str,"vec4(");

						if(height_axis==HGL_AXIS_NONE)							//非高度图
						{
							::strcat(pos_str,HGL_VS_VERTEX);
							::strcat(pos_str,",0.0,1.0);\n");
						}
						else if(height_axis==HGL_AXIS_X)						//X轴向上
						{
							::strcat(pos_str,tex_sampler[mtcHeight]);
							::strcat(pos_str,".r,");

							::strcat(pos_str,HGL_VS_VERTEX);
							::strcat(pos_str,",1.0);\n");
						}
						else if(height_axis==HGL_AXIS_Y)						//Y轴向上
						{
							::strcat(pos_str,HGL_VS_VERTEX);
							::strcat(pos_str,".x,");

							::strcat(pos_str,tex_sampler[mtcHeight]);
							::strcat(pos_str,".r,");

							::strcat(pos_str,HGL_VS_VERTEX);
							::strcat(pos_str,".y,1.0);\n");
						}
						else if(height_axis==HGL_AXIS_Z)						//Z轴向上
						{
							::strcat(pos_str,HGL_VS_VERTEX);
							::strcat(pos_str,",");

							::strcat(pos_str,tex_sampler[mtcHeight]);
							::strcat(pos_str,".r,1.0);\n");
						}
						else
						{
							LOG_ERROR(OS_TEXT("vs::add_end(),生成高度图高度坐标时，发现向上轴标识无法理解"));
						}
					}
					else
					if(vertex_type==3)
					{
						::strcat(pos_str,"vec4(");
						::strcat(pos_str,HGL_VS_VERTEX);
						::strcat(pos_str,",1.0);\n");
					}
					else
					if(vertex_type==4)
					{
						::strcat(pos_str,HGL_VS_VERTEX);
						::strcat(pos_str,";\n");
					}
					else
					{
						LOG_ERROR(OS_TEXT("错误的顶点格式!"));
						return(false);
					}

					add();
					add(pos_str);
					add();

					if(!mvp_matrix)
						add_format("\tgl_Position=Position;\n");
					else
						add_format("\tgl_Position=%s*Position;\n",HGL_VS_MVP_MATRIX);
				}

				if(in_normal)
				{
					add();

					add_format("\tVP=normalize(%s-%s*Position);\n",HGL_VS_LIGHT_POSITION,HGL_VS_NORMAL_MATRIX);

					add_format("\tMVNormal=normalize(%s*%s);\n",HGL_VS_NORMAL_MATRIX,HGL_VS_NORMAL);

					add();

					add_format("\t%s=%s+max(0.0,dot(MVNormal,VP));\n",HGL_FS_LIGHT_INTENSITY,HGL_VS_GLOBAL_LIGHT_INTENSITY);
				}

				return(true);
			}
		}//namespace shadergen

		/**
		* 生成Vertex Shader代码
		* @param able 可渲染数据指针
		* @param mvp 是否有mvp矩阵
		* @return 生成的Vertex Shader代码
		* @return NULL 生成失身为
		*/
#ifdef _DEBUG
		char *MakeVertexShader(Renderable *able,bool mvp,RenderState *state,const char16_t *filename)
#else
		char *MakeVertexShader(Renderable *able,bool mvp,RenderState *state)
#endif//
		{
			if(!able)return(nullptr);

			Material *mat=able->GetMaterial();

			if(!mat)return(nullptr);

			shadergen::vs code;

			code.add_version(330);		//OpenGL 3.3

			if(state->mvp)
			{
				code.add_mvp(((state->vertex_normal||state->normal_map)&&state->lighting)?true:false);		//需要法线,则必须传modelview
				code.add();
			}

			VertexBufferBase *vb_vertex=able->GetVertexBuffer(vbtVertex);

			//顶点
			{
				code.add_in_vertex(vb_vertex->GetComponent());
				code.add();
			}

			//颜色
			if(state->vertex_color)					//使用顶点颜色
			{
				code.add_in_color(able->GetVertexColorFormat());
				code.add();
			}
//				else
//				if(state->color_material)				//使用独立颜色传入
//				{
//					code.add_uniform_color(4);			//使用uniform传入颜色
//				}	//

			//灯光
			if(state->lighting)
			{
				code.add_uniform_vec3(HGL_VS_LIGHT_POSITION);
				code.add();

				//法线
				if(state->vertex_normal)				//使用顶点法线
				{
					code.add_in_normal();
					code.add();
				}
			}

			//纹理
			if(mat->GetTextureNumber())					//如果有贴图
			{
				int tex_count=0;

				//高度图
				if(state->height_map)					//如果是高度图，则是拿vertex当高度图坐标、以及各种纹理坐标
				{										//高度图网格永远画在(0,0)-(1,1)的范围内，所以这个坐标同时可以直接用于高图度的纹理坐标，以及漫反射、法线、光照、阴影等贴图上。
					if(vb_vertex->GetComponent()!=2)
					{
						LOG_ERROR(OS_TEXT("使用高度图，但传入的顶点坐标不是2维数据."));
						return(nullptr);
					}

					int hm_map_count=0;

					code.add_in_texture(VertexBufferName[vbtVertex],2,mtcHeight);

					if(mat->GetTexture(mtcDiffuse))					//漫反射，绘制坐标即贴图坐标
					{
						code.add_in_texcoord(vbtVertex,VertexBufferName[vbtVertex],2,mtcDiffuse);
						hm_map_count++;
					}

					if(hm_map_count)
						code.add_out_texcoord(2,mtcHeight,VertexBufferName[vbtVertex]);		//将顶点坐标传给漫反射、镜片光、高光等纹理坐标

					code.add();

					code.set_height_axis(mat->GetHeightAxis());
				}

				for(int i=0;i<mtcMax;i++)							//增加贴图坐标
				{
					VertexBufferType vbt;
					VertexBufferBase *vb=able->GetTexCoord(i,&vbt);

					if(!vb)continue;

					code.add_in_texcoord(vbt,VertexBufferName[vbt],vb->GetComponent(),i);		//第一个参数用vbt而不用i是为了让shader中的texcoord?编号与程序中填写的缓冲区编号一致，便于调试查看

					code.add_out_texcoord(vb->GetComponent(),i,VertexBufferName[vbt]);			//一般是输出到fragment shader用

					tex_count++;
				}

				if(tex_count)		//没有加贴图不加这个空行
					code.add();
			}

			code.add_main_begin();

			code.add_end();							//收尾各方面的处理

			code.add_main_end();

#ifdef _DEBUG
			code.debug_out(filename);
#endif//_DEBUG

			return code.get();
		}//char *MakeVertexShader(Renderable *able,bool ltw)
	}//namespace graph
}//namespace hgl
