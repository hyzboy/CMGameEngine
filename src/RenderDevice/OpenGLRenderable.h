#ifndef HGL_GRAPH_OPENGL_CORE_RENDERABLE_INCLUDE
#define HGL_GRAPH_OPENGL_CORE_RENDERABLE_INCLUDE

#include<hgl/graph/Renderable.h>
#include<hgl/CompOperator.h>
#include<string.h>

namespace hgl
{
	namespace graph
	{
		struct RenderState
		{
			bool mvp;					//是否包含MVP矩阵

			bool vertex_color;			//是否包含顶点颜色
			bool vertex_normal;			//是否包含顶点法线
			bool vertex_tangent;        //是否包含顶点切线

			bool diffuse_map;			//是否包含漫反射贴图
			bool normal_map;			//是否包含法线贴图
			bool tangent_map;           //是否包含切线贴图

			bool color_material;		//是否包含颜色材质
			bool alpha_test;			//是否使用Alpha测试
			bool outside_discard;		//是否使用出界放弃

			bool height_map;			//是否是高度图

			uint8 vertex_color_format;	//顶点色格式
			uint8 vertex_coord;			//顶点坐标维数
			bool  tex[mtcMax];			//这一通道是否有贴图
			uint8 tex_coord[mtcMax];	//这一通道是贴图坐标维数

			//仅测试用
			bool lighting;				//是否有光照

		public:

			int Comp(const RenderState &rs)const
			{
				return memcmp(this,&rs,sizeof(RenderState));
			}

			CompOperator(const RenderState &,Comp);
		};//RenderState

		class OpenGLCoreRenderable:public Renderable
		{
			unsigned int vao;

			int *location;		//shader绑定变量地址

			RenderState state;			//渲染状态

		private:

			bool MakeRenderState(bool);	//生成渲染状态

		public:

			OpenGLCoreRenderable();
			virtual ~OpenGLCoreRenderable();

			bool SetVertexBuffer(VertexBufferType,VertexBufferBase *);						///<设置顶点缓冲区数据

			bool SetShaderLocation(VertexBufferType,unsigned int);							///<设定与Shader变量的关联
			void ClearShaderLocation();														///<清除与Shader变量的关联

			bool Bind();																	///<使用这个VAO

			const RenderState *GetRenderState()const{return &state;}						///<取得渲染状态

#ifdef _DEBUG
			Shader *AutoCreateShader	(bool mvp=true,const os_char *filename=nullptr);	///<自动创建着色程序
#else
			Shader *AutoCreateShader	(bool mvp=true);									///<自动创建着色程序
#endif//_DEBUG
		};//class OpenGLCoreRenderable
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_OPENGL_CORE_RENDERABLE_INCLUDE
