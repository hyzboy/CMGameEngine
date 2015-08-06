#include<glew/include/GL/glew.h>
#include"GLSL.h"
#include<hgl/graph/Render.h>
#include<hgl/graph/Texture.h>
#include<hgl/graph/Material.h>
#include<hgl/graph/VertexBuffer.h>
#include<hgl/graph/InlineRenderable.h>
#include<hgl/LogInfo.h>
#include"OpenGLRenderable.h"
namespace hgl
{
	/**
	* 可改进项：

		2.2D渲染坐标系不调整状态无需modelview矩阵，现传递的是identity，可以想办法免去。
	*/

	namespace graph
	{
		namespace OpenGLCore
		{
			void PutOpenGLInfo();	//输出OpenGL特性信息
			void InitScissor();		//初始化裁剪区
			void InitTexture();		//初始化贴图
			void CloseTexture();	//关闭贴图
			void InitBuffer();		//初始化缓冲区
			void InitFace();		//初始化面处理
		}//namespace OpenGLCore

		using namespace OpenGLCore;

		void InitVertexBuffer();	//初始化顶点缓冲区
		bool BindTexture(int active,unsigned int type,unsigned int index);
		void InitShaderStorage();
		void ClearShaderStorage();
// 		void InitFontStorage();
// 		void ClearFontStorage();

		void InitInlineRenderable();
		void ClearInlineRenderable();

		/**
		* 初始化渲染器
		*/
		void InitRender()
		{
			PutOpenGLInfo();
			InitScissor();
			InitTexture();
			InitBuffer();
			InitFace();

			InitVertexBuffer();

			InitShaderStorage();
// 			InitFontStorage();

			InitInlineRenderable();
		}

		void CloseRender()
		{
			ClearInlineRenderable();

// 			ClearFontStorage();
			ClearShaderStorage();
			CloseTexture();
		}
	}//namespace graph

	namespace graph
	{
		void SetPolygonMode(Material *mat)
		{
			if(mat)
			{
				uint draw_face,fill_mode,depth_func;

				mat->GetDrawMode(draw_face,fill_mode);

				SetPolygonMode(draw_face,fill_mode);

				const uint cull_face=mat->GetCullFace();

				if(cull_face==0)
					CloseCullFace();
				else
					OpenCullFace(cull_face);

				if(mat->GetDepthTest(depth_func))
					OpenDepthTest(depth_func);
				else
					CloseDepthTest();

				SetDepthMask(mat->GetDepthMask());
			}
			else
			{
				SetPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

				CloseCullFace();

				OpenDepthTest(GL_LESS);
				SetDepthMask(true);
			}
		}

		bool BindShaderVertexAttrib(OpenGLCoreRenderable *obj,GLSL *glsl,int mtc)
		{
			VertexBufferBase *vb=obj->GetVertexBuffer((VertexBufferType)mtc);				//取得缓冲区

			if(!vb)return(true);															//没有这个缓冲区

			int location=glsl->GetAttribLocation(VertexBufferName[mtc]);					//取得缓冲区对应shader属性地址

			if(location==-1)																//如果取得地址失败
			{
				LOG_HINT(u8"buffer \""+UTF8String(VertexBufferName[mtc])+u8"\" attrib location =-1");
				return(false);
			}

			obj->SetShaderLocation((VertexBufferType)mtc,location);							//设定缓冲区对应glsl属性

			return(true);
		}

		bool BindShaderVertexAttrib(OpenGLCoreRenderable *obj,GLSL *glsl,const RenderState *state)
		{
			//本来这段可以写到OpenGLCoreRenderable类中的，为了一些变数，写在这里
			obj->ClearShaderLocation();														//清除原有shader与顶点缓冲区的绑定

			//非贴图坐标数据
			for(int i=vbtVertex;i<vbtDiffuseTexCoord;i++)
			{
				if(i==vbtNormal
				&&!state->lighting)continue;												//有法线但确定不用光照，则不传递法线

				if(!BindShaderVertexAttrib(obj,glsl,i))
				{
					LOG_PROBLEM(OS_TEXT("BindShaderVertexAttrib error,vbt=")+OSString(i));
					return(false);
				}
			}

			//设置贴图坐标数据
			for(int i=0;i<mtcMax;i++)
			{
				VertexBufferType vbt;

				if(obj->GetTexCoord(i,&vbt))
					BindShaderVertexAttrib(obj,glsl,vbt);
			}

			obj->Bind();																	//缓定glsl与缓冲区

			return(true);
		}

		bool BindShaderUniform(Renderable *able,GLSL *glsl,const Matrix4f *modelview,const RenderState *state)
		{
			const Material *mat=able->GetMaterial();

			//颜色
			if(state->color_material)
			{
				glsl->Shader::SetUniform4fv(HGL_FS_COLOR,mat->GetColor());
			}

			//AlphaTest
			if(state->alpha_test)
			{
				glsl->Shader::SetUniform1f(HGL_FS_ALPHA_TEST,mat->GetAlphaTest());
			}

			//灯光
			if(state->lighting)
			{
				glsl->Shader::SetUniform3fv(HGL_VS_LIGHT_POSITION,mat->GetLightPosition());
				glsl->Shader::SetUniform1f(HGL_VS_GLOBAL_LIGHT_INTENSITY,mat->GetGlobalLightIntensity());
			}

			return(true);
		}

		bool BindShaderTexture(Renderable *able,GLSL *glsl)
		{
			Material *mat=able->GetMaterial();

			int tex_count=0;

			if(mat->GetTextureNumber())					//如果有贴图
			{
				for(int i=0;i<mtcMax;i++)
				{
					Texture *tex=mat->GetTexture(i);									//取指定通道贴图

					if(!tex)continue;													//贴图不存在

					BindTexture(tex_count,tex->GetType(),tex->GetIndex());				//绑定贴图

					if(!glsl->Shader::SetUniform1i(MaterialTextureName[i],tex_count))	//设定贴图对应索引
					{
						LOG_PROBLEM(u8"attach Shader sampler \""+UTF8String(MaterialTextureName[i])+u8"\" to texture "+UTF8String(tex_count)+u8" error!");
						return(false);
					}

					tex_count++;
				}
			}

			return(true);
		}

		bool BindShaderMatrix(GLSL *glsl,const Matrix4f *projection,const Matrix4f *modelview,const RenderState *state)
		{
			if(!state->mvp)
				return(true);

			if(!projection)
				return(false);

			if(modelview)
			{
				const Matrix4f mvp=(*projection)*(*modelview);

				if(!glsl->Shader::SetUniformMatrix4fv(HGL_VS_MVP_MATRIX,mvp))
					return(false);
			}
			else	//没modelview时将projection传为mvp
			{
				if(!glsl->Shader::SetUniformMatrix4fv(HGL_VS_MVP_MATRIX,*projection))
					return(false);
			}

			if((state->vertex_normal||state->normal_map)&&state->lighting)	//需要法线
			{
				//const Matrix3f normal_matrix=*modelview;		//法线矩阵为3x3



				//if(!glsl->Shader::SetUniformMatrix3fv(HGL_VS_NORMAL_MATRIX,normal_matrix.data()))
					//return(false);
			}

			return(true);
		}

		void SetBlend(Material *mtl)
		{
			if(!mtl)
			{
				glDisable(GL_BLEND);
				return;
			}

			bool blend;
			BlendMode mode;

			blend=mtl->GetBlend(mode);

			if(!blend)
			{
				glDisable(GL_BLEND);
				return;
			}

			glEnable(GL_BLEND);

			glBlendFuncSeparate(mode.rgb.src,
								mode.rgb.dst,
								mode.alpha.src,
								mode.alpha.dst);

			glBlendEquationSeparate(mode.rgb.func,
									mode.alpha.func);
		}

		/**
		* 直接渲染一个对象
		* @param obj 可渲染对象
		* @param mvp Projection*ModelView矩阵数据
		* @param modelview ModelView矩阵数据
		* @return 是否执行成功
		*/
		bool DirectRender(Renderable *obj,const Matrix4f *projection,const Matrix4f *modelview)
		{
			if(!obj)return(false);

			GLSL *		glsl		=(GLSL *)obj->GetShader();	//取得glsl
			Material *	mtl			=obj->GetMaterial();		//取得材质
			uint		draw_prim	=obj->GetPrimitive();		//取得图元类型
			int			draw_start;
			int			draw_count;

			if(!glsl||!mtl)return(false);

			VertexBufferBase *vb_index=obj->GetVertexBuffer(vbtIndex);

			//确认绘制数据量
			/*if(vb_index)		//使用索引绘制
			{
				draw_count=vb_index->GetCount();					//取得所需绘制的顶点数
			}
			else
			{
				VertexBufferBase *vb=obj->GetVertexBuffer(vbtVertex);

				if(!vb)return(false);

				draw_count=vb->GetCount();							//取得所需绘制的顶点数
			}*/

			obj->GetDrawCount(draw_start,draw_count);

			if(draw_count<=0)return(false);							//如果数量为0

			SetPolygonMode(mtl);

			//绑定shader
			{
				const RenderState *state=((OpenGLCoreRenderable *)obj)->GetRenderState();		//渲染状态

				glsl->Use();																	//启用glsl

				if(!BindShaderVertexAttrib((OpenGLCoreRenderable *)obj,glsl,state))				//绑定shader顶点属性
				{
					LOG_PROBLEM(OS_TEXT("BindShaderVertexAttrib error"));
					return(false);
				}

				if(!BindShaderUniform(obj,glsl,modelview,state))								//绑定shader一致变量
				{
					LOG_PROBLEM(OS_TEXT("BindShaderUniform error"));
					return(false);
				}

				if(!BindShaderMatrix(glsl,projection,modelview,state))							//绑定shader矩阵
				{
					LOG_PROBLEM(OS_TEXT("BindShaderMatrix error"));
					return(false);
				}

				if(!BindShaderTexture(obj,glsl))												//绑定shader与贴图
				{
					LOG_PROBLEM(OS_TEXT("BindShaderTexture error"));
					return(false);
				}
			}

			//设置混合
			SetBlend(mtl);

			//绘制
			{
				if(vb_index)
					glDrawElements(draw_prim,draw_count,vb_index->GetDataType(),(const void *)(draw_start*vb_index->GetDataBytes()));
				else
					glDrawArrays(draw_prim,draw_start,draw_count);								//绘制数据
			}

			return(true);
		}//bool DirectRender(Renderable *obj,const Matrix4f *proj,const Matrix4f *modelview)
	}//namespace graph
}//namespace hgl
