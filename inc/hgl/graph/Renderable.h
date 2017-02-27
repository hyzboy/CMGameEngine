#ifndef HGL_GRAPH_RENDERABLE_INCLUDE
#define HGL_GRAPH_RENDERABLE_INCLUDE

#include<hgl/graph/Shader.h>
#include<hgl/graph/Material.h>
#include<hgl/graph/Primivate.h>
#include<hgl/graph/VertexArray.h>
#include<hgl/graph/RenderState.h>
namespace hgl
{
	namespace graph
	{
		class ShaderStorage;

		/**
		* 可渲染对象
		*/
		class Renderable
		{
            void Init();
            void Clear();

		protected:

            VertexArray *va;                                                                                           		///<顶点数据

			Material *material;																							    ///<材质

			VertexBufferType TextureChannels[mtcMax];																	    ///<贴图通道对应表

			int DrawStart,DrawCount;																					    ///<可绘制数量

        protected:

            uint        vao;
            int *       location;                                                                                           ///<shader绑定变量地址
            RenderState state;                                                                                              ///<渲染状态
            Shader *    shader;                                                                                             ///<对应shader

            Shader *    bind_shader;                                                                                        ///<上一次绑定的shader

            bool        MakeRenderState(bool);                                                                              ///<生成渲染状态

		public:

            Renderable(VertexArray *,Material *mtl=nullptr);
			~Renderable();

		public:

			uint				GetPrimitive		()const					{return va?va->GetPrimitive():0;}				///<取得要绘制的图元类型

			bool				SetMaterial			(Material *);															///<设置材质数据
			Material * 			GetMaterial			()const					{return material;}								///<取得材质指针

            VertexBufferBase *  GetVertexBuffer     (VertexBufferType vbt)	{return va?va->GetVertexBuffer(vbt):nullptr;} 	///<取得对应顶点缓冲区数据

            int                 GetVertexCompoment  ()const{return va?va->GetVertexCompoment():0;}                      	///<取得顶点数据坐标轴数
            PixelCompoment      GetVertexPixelCompoment()const{return va?va->GetVertexPixelCompoment():HGL_PC_NONE;}       	///<取得顶点数据成份属性

			bool				SetDrawCount		(int,int);																///<设置要绘制的数据数量
			bool		        GetDrawCount		(int &,int &);															///<取得指定的要绘制的数据数量

			bool				SetTexCoord			(int mtc,VertexBufferType);												///<设定贴图坐标对应缓冲区
			VertexBufferBase *	GetTexCoord			(int mtc,VertexBufferType *vbt=0);										///<取得贴图坐标对应的缓冲区

        public: //着色程序

            void                SetShader           (Shader *);                                                             ///<设置着色程序
            Shader *            GetShader           ()const{return shader;}                                                 ///<取得着色程序

            bool                SetShaderLocation   (VertexBufferType, unsigned int);                                       ///<设定与Shader变量的关联
            void                ClearShaderLocation ();                                                                     ///<清除与Shader变量的关联

            bool                Bind                (Shader *);                                                             ///<绑定VAO数据
            Shader *            GetBindShader       ()const{return bind_shader;}                                            ///<取得上一次绑定的shader
            bool                Use                 ();                                                                     ///<使用这个VAO+Shader渲染

            const RenderState * GetRenderState      ()const { return &state; }                                              ///<取得渲染状态

#ifdef _DEBUG
			Shader *			AutoCreateShader	(bool mvp=true,ShaderStorage *storage=nullptr,const os_char *debug_outname=nullptr);
#else
            Shader *			AutoCreateShader	(bool mvp=true,ShaderStorage *storage=nullptr);							///<自动创建Shader
#endif//#endif//_DEBUG

			
		};//class Renderable
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_RENDERABLE_INCLUDE
