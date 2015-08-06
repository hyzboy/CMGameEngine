#include"OpenGLRenderable.h"
#include"GLSL.h"
#include<hgl/type/Smart.h>
#include<hgl/type/Map.h>
#include<glew/include/GL/glew.h>
/*
In GL_EXT_direct_state_access there was a function named glVertexArrayVertexAttribOffsetEXT. I do not see an equivalent in GL_ARB_direct_state_access. I'm trying to accomplish the following using the new core DSA functions:

GLuint vao = 0;
glGenVertexArrays(1, &vao);
glBindVertexArray(vao);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
glEnableVertexArrayAttribEXT(vao, 0);
glEnableVertexArrayAttribEXT(vao, 1);
glVertexArrayVertexAttribOffsetEXT(vao, buffers[0], 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
glVertexArrayVertexAttribOffsetEXT(vao, buffers[1], 1, 2, GL_FLOAT, GL_FALSE, 0, 0);

I have tried the following, but it does work:

GLuint vao = 0;
glCreateVertexArrays(1, &vao);
glVertexArrayElementBuffer(vao, buffers[3]);
glEnableVertexArrayAttrib(vao, 0);
glEnableVertexArrayAttrib(vao, 1);
glVertexArrayVertexBuffer(vao, 0, buffers[0], 0, 0);
glVertexArrayVertexBuffer(vao, 1, buffers[1], 0, 0);
glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, 0);
*/

namespace hgl
{
	namespace graph
	{
		const uint OpenGLCorePrim[]=
		{
			GL_POINTS,
			GL_LINES,
			GL_LINE_STRIP,
			GL_LINE_LOOP,
			GL_TRIANGLES,
			GL_TRIANGLE_STRIP,
			GL_TRIANGLE_FAN,
//			GL_QUADS,
//			GL_QUAD_STRIP,
			GL_LINES_ADJACENCY,
			GL_LINE_STRIP_ADJACENCY,
			GL_TRIANGLES_ADJACENCY,
			GL_TRIANGLE_STRIP_ADJACENCY,
			GL_PATCHES
		};//const uint OpenGLCorePrim[]

		const int OpenGLCorePrimCount=sizeof(OpenGLCorePrim)/sizeof(uint);

		bool CheckPrim(uint prim)
		{
			const uint *p=OpenGLCorePrim;

			for(int i=0;i<OpenGLCorePrimCount;i++)
				if(*p++==prim)return(true);

			return(false);
		}

		Renderable *CreateRenderable()
		{
			return(new OpenGLCoreRenderable);
		}
	}//namespace graph

	namespace graph
	{
		//shader 仓库管理
		//1.永不释放

		MapObject<RenderState,Shader> ShaderStorage;						///<shader仓库

		void InitShaderStorage()
		{
		}

		void ClearShaderStorage()
		{
			ShaderStorage.Clear();
		}

		Shader *CreateShader(Renderable *,bool,RenderState *
#ifdef _DEBUG
			,const os_char *
#endif//_DEBUG
		);
	}//namespace graph

	namespace graph
	{
		OpenGLCoreRenderable::OpenGLCoreRenderable()
		{
			glGenVertexArrays(1,&vao);

			location=new int[HGL_MAX_VERTEX_ATTRIBS];

			for(int i=0;i<HGL_MAX_VERTEX_ATTRIBS;i++)
				location[i]=-1;
		}

		OpenGLCoreRenderable::~OpenGLCoreRenderable()
		{
			delete[] location;

			glDeleteVertexArrays(1,&vao);
		}

		/**
		* 设置顶点缓冲区数据
		* @param vbt 顶点缓冲区类型
		* @param vb 数据缓冲区
		* @return 是否设置成功
		*/
		bool OpenGLCoreRenderable::SetVertexBuffer(VertexBufferType vbt,VertexBufferBase *vb)
		{
			if(!Renderable::SetVertexBuffer(vbt,vb))
				return(false);

			vb->CreateVertexBuffer(vbt==vbtIndex?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER);

			return(true);
		}

		/**
		* 设置缓冲区对应的shader变量
		* @param vbt 顶点缓冲区类型
		* @param shader_locaiton shader变量地址
		* @param enabled 是否立即启用
		*/
		bool OpenGLCoreRenderable::SetShaderLocation(VertexBufferType vbt,unsigned int shader_location)
		{
			if(vbt<vbtVertex||vbt>=HGL_MAX_VERTEX_ATTRIBS)
			{
				LOG_ERROR(OS_TEXT("OpenGLCoreRenderable::SetShaderLocation设置的数据类型错误：")+OSString(vbt));
				return(false);
			}

			location[vbt]=shader_location;

			return(true);
		}

		void OpenGLCoreRenderable::ClearShaderLocation()
		{
			glBindVertexArray(vao);

			for(int i=vbtIndex;i<HGL_MAX_VERTEX_ATTRIBS;i++)
			{
				if(location[i]==-1)continue;

				glDisableVertexAttribArray(location[i]);
				location[i]=-1;
			}
		}

		bool OpenGLCoreRenderable::Bind()
		{
			if(!vao)return(false);

			glBindVertexArray(vao);

			for(int i=vbtVertex;i<HGL_MAX_VERTEX_ATTRIBS;i++)
			{
				if(location[i]==-1)continue;

				if(!vertex_buffer[i])
				{
					LOG_ERROR(OS_TEXT("Shader Location <")+OSString(i)+OS_TEXT(">要求的缓冲区没有数据"));
					return(false);
				}

				vertex_buffer[i]->BindVertexBuffer();
				glVertexAttribPointer(location[i],vertex_buffer[i]->GetComponent(),vertex_buffer[i]->GetDataType(),GL_FALSE,0,0);
				glEnableVertexAttribArray(location[i]);
			}

			if(vertex_buffer[vbtIndex])
				vertex_buffer[vbtIndex]->BindVertexBuffer();

			return(true);
		}

		/**
		* 生成渲染状态
		*/
		bool OpenGLCoreRenderable::MakeRenderState(bool mvp)
		{
			VertexBufferBase *vb_vertex	=GetVertexBuffer(vbtVertex);

			if(!vb_vertex)return(false);						//没顶点，画不了

			memset(&state,0,sizeof(state));

			state.mvp=mvp;

			state.vertex_normal			=GetVertexBuffer(vbtNormal);
			state.vertex_color			=GetVertexBuffer(vbtColor);
            state.vertex_tangent        =GetVertexBuffer(vbtTangent);

			state.diffuse_map			=GetTexCoord(mtcDiffuse);
			state.normal_map			=GetTexCoord(mtcNormal);
            state.tangent_map           =GetTexCoord(mtcTangent);

			state.color_material		=material->GetColorMaterial();
			state.alpha_test			=material->GetAlphaTest()>0;
			state.outside_discard		=material->GetOutsideDiscard();

			state.vertex_color_format	=GetVertexColorFormat();
			state.vertex_coord			=vb_vertex->GetComponent();

			state.height_map			=material->GetTexture(mtcHeight);

			for(int i=0;i<mtcMax;i++)
			{
				if(material->GetTexture(i))
					state.tex[i]=true;

				VertexBufferBase *vb=GetTexCoord(i,0);

				if(vb)
					state.tex_coord[i]=vb->GetComponent();
			}

			state.lighting				=material->GetLight();

			if(!CheckPrim(prim))
			{
				LOG_ERROR(OS_TEXT("错误的图元类型：")+OSString(prim));
				return(false);
			}

			return(true);
		}

		Shader *OpenGLCoreRenderable::AutoCreateShader(bool mvp
#ifdef _DEBUG
			,const os_char *shader_filename
#endif//_DEBUG
		)
		{
			if(shader)											//如果有shader存在
			{
				RenderState back_state=state;					//备份旧状态

				if(!MakeRenderState(mvp))						//生成新的状态
					return(nullptr);

				if(back_state==state)							//如果新旧状态一样
					return(shader);

				shader=0;										//清除旧的shader
			}
			else
			{
				if(!MakeRenderState(mvp))						//生成新的状态
					return(nullptr);
			}

			Shader *new_shader;

			if(!ShaderStorage.Get(state,new_shader))			//如果仓库中有状态一样的，则直接从仓库中取，而不重新创建
			{
				new_shader=CreateShader(this,mvp,&state
	#ifdef _DEBUG
					,shader_filename
	#endif//_DEBUG
				);

				ShaderStorage.Add(state,new_shader);			//加入到仓库
			}

			shader=new_shader;

			return shader;
		}
	}//namespace graph
}//namespace hgl
