#include"OpenGLRenderable.h"
#include"GLSL.h"
#include<hgl/type/Smart.h>
#include<hgl/type/Map.h>
#include<glew/include/GL/glew.h>

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
		/**
		* 设置顶点缓冲区数据
		* @param vbt 顶点缓冲区类型
		* @param vb 数据缓冲区
		* @return 是否设置成功
		*/
		bool OpenGLCoreRenderable::SetVertexBuffer(VertexBufferType vbt, VertexBufferBase *vb)
		{
			if (!Renderable::SetVertexBuffer(vbt, vb))
				return(false);

			vb->CreateVertexBuffer(vbt == vbtIndex ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER);

			return(true);
		}
	}//namespace graph

	namespace graph
	{
		OpenGLCoreRenderableBinding::OpenGLCoreRenderableBinding(OpenGLCoreRenderable *r, GLSL *g)
		{
			renderable = r;
			shader=g

            glCreateVertexArrays(1,&vao);

			location=new int[HGL_MAX_VERTEX_ATTRIBS];

			for(int i=0;i<HGL_MAX_VERTEX_ATTRIBS;i++)
				location[i]=-1;
		}

		OpenGLCoreRenderableBinding::~OpenGLCoreRenderableBinding()
		{
			delete[] location;

			glDeleteVertexArrays(1,&vao);
		}

		/**
		* 设置缓冲区对应的shader变量
		* @param vbt 顶点缓冲区类型
		* @param shader_locaiton shader变量地址
		* @param enabled 是否立即启用
		*/
		bool OpenGLCoreRenderableBinding::SetShaderLocation(VertexBufferType vbt,unsigned int shader_location)
		{
			if(vbt<vbtVertex||vbt>=HGL_MAX_VERTEX_ATTRIBS)
			{
				LOG_ERROR(OS_TEXT("OpenGLCoreRenderable::SetShaderLocation设置的数据类型错误：")+OSString(vbt));
				return(false);
			}

			location[vbt]=shader_location;

			return(true);
		}

		void OpenGLCoreRenderableBinding::ClearShaderLocation()
		{
			for(int i=vbtIndex;i<HGL_MAX_VERTEX_ATTRIBS;i++)
			{
				if(location[i]==-1)continue;

                glDisableVertexArrayAttrib(vao,i);
				location[i]=-1;
			}
		}

		bool OpenGLCoreRenderableBinding::Bind(int shader)
		{
			if(!vao)return(false);

            int stream=0;

			for(int i=vbtVertex;i<HGL_MAX_VERTEX_ATTRIBS;i++)
			{
				if(location[i]==-1)continue;

				if(!vertex_buffer[i])
				{
					LOG_ERROR(OS_TEXT("Shader Location <")+OSString(i)+OS_TEXT(">要求的缓冲区没有数据"));
					return(false);
				}

                glVertexArrayAttribBinding(vao,                                             //vao obj
                                           location[i],                                     //attrib index
                                           stream);                                         //binding index

                if(vertex_buffer[i]->GetDataType()==HGL_INT)    glVertexArrayAttribIFormat(  vao,location[i],vertex_buffer[i]->GetComponent(),vertex_buffer[i]->GetDataType(),0);else
                if(vertex_buffer[i]->GetDataType()==HGL_DOUBLE) glVertexArrayAttribLFormat(  vao,location[i],vertex_buffer[i]->GetComponent(),vertex_buffer[i]->GetDataType(),0);else
                                                                glVertexArrayAttribFormat(   vao,                                   //vao obj
                                                                                             location[i],                           //attrib index
                                                                                             vertex_buffer[i]->GetComponent(),      //size
                                                                                             vertex_buffer[i]->GetDataType(),       //type
                                                                                             GL_FALSE,                              //normalized
                                                                                             0);                                    //relative offset

                glEnableVertexArrayAttrib(vao,                                          //vao obj
                                          location[i]);                                 //attrib index

                glVertexArrayVertexBuffer(vao,                                          //vao obj
                                          stream,                                       //binding index
                                          vertex_buffer[i]->GetBufferIndex(),           //buffer
                                          0,                                            //offset
                                          vertex_buffer[i]->GetStride());               //stride

                ++stream;
            }

 			if(vertex_buffer[vbtIndex])
                 glVertexArrayElementBuffer(vao,                                         //vao obj
                                            vertex_buffer[vbtIndex]->GetBufferIndex());  //buffer

			glsl = shader;
			return(true);
		}

		bool OpenGLCoreRenderableBinding::Use()
		{
			if(!vao)return(false);

            glBindVertexArray(vao);
			return(true);
		}

		/**
		* 生成渲染状态
		*/
		bool OpenGLCoreRenderableBinding::MakeRenderState(bool mvp)
		{
			VertexBufferBase *vb_vertex	=renderable->GetVertexBuffer(vbtVertex);

			if(!vb_vertex)return(false);						//没顶点，画不了

			memset(&state,0,sizeof(state));

			state.mvp=mvp;

			state.vertex_normal			= renderable->GetVertexBuffer(vbtNormal);
			state.vertex_color			= renderable->GetVertexBuffer(vbtColor);
            state.vertex_tangent        = renderable->GetVertexBuffer(vbtTangent);

			state.diffuse_map			= renderable->GetTexCoord(mtcDiffuse);
			state.normal_map			= renderable->GetTexCoord(mtcNormal);
            state.tangent_map           = renderable->GetTexCoord(mtcTangent);

			state.color_material		= renderable->material->GetColorMaterial();
			state.alpha_test			= renderable->material->GetAlphaTest()>0;
			state.outside_discard		= renderable->material->GetOutsideDiscard();

			state.vertex_color_format	= renderable->GetVertexColorFormat();
			state.vertex_coord			= vb_vertex->GetComponent();

			state.height_map			= renderable->material->GetTexture(mtcHeight);

			for(int i=0;i<mtcMax;i++)
			{
				if(renderable->material->GetTexture(i))
					state.tex[i]=true;

				VertexBufferBase *vb= renderable->GetTexCoord(i,0);

				if(vb)
					state.tex_coord[i]=vb->GetComponent();
			}

			state.lighting				= renderable->material->GetLight();

			if(!CheckPrim(renderable->GetPrimitive()))
			{
				LOG_ERROR(OS_TEXT("错误的图元类型：")+OSString(renderable->GetPrimitive()));
				return(false);
			}

			return(true);
		}

		Shader *OpenGLCoreRenderableBinding::AutoCreateShader(bool mvp
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
