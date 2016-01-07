#ifndef HGL_GRAPH_RENDERABLE_INCLUDE
#define HGL_GRAPH_RENDERABLE_INCLUDE

#include<hgl/graph/Shader.h>
#include<hgl/graph/Material.h>
#include<hgl/graph/Primivate.h>
#include<hgl/graph/RenderableData.h>
namespace hgl
{
	namespace graph
	{
		/**
		* 可渲染对象
		*/
		class Renderable
		{
		protected:

            RenderableData *data;                                                                                       ///<数据

			Material *material;																							///<材质
			bool mtl_private;																							///<材质是否私有

			VertexBufferType TextureChannels[mtcMax];																	///<贴图通道对应表

			uint prim;																									///<绘制的图元类型

			int DrawStart,DrawCount;																					///<可绘制数量

        private:

            friend Renderable *CreateRenderable(RenderableData *);

            Renderable(RenderableData *);                   //请通过CreateRenderableData来创建可渲染对像,函数在Render.h中定义

		public:

			virtual ~Renderable();

		public:

			virtual void						SetPrimitive		(uint dp)					{prim=dp;}									///<设置的绘制的图元类型
			virtual uint						GetPrimitive		()const						{return prim;}								///<取得要绘制的图元类型

			virtual bool						SetMaterial			(Material *,bool);														///<设置材质数据
			virtual Material * 					GetMaterial			()const						{return material;}							///<取得材质指针


			virtual bool						SetDrawCount		(int,int);																///<设置要绘制的数据数量
			virtual bool		                GetDrawCount		(int &,int &);															///<取得指定的要绘制的数据数量

			virtual bool						SetTexCoord			(int mtc,VertexBufferType);												///<设定贴图坐标对应缓冲区
			virtual VertexBufferBase *			GetTexCoord			(int mtc,VertexBufferType *vbt=0);										///<取得贴图坐标对应的缓冲区

        public: //着色程序

            virtual void                        SetShader           (Shader *)=0;                                                           ///<设置着色程序
            virtual Shader *                    GetShader           ()const=0;                                                              ///<取得着色程序

            /**
            * 自动创建生成shader
            * @param mvp 渲染时是否使用projection矩阵与modelview矩阵
            * @return 创建好的shader程序
            */
            virtual Shader *                    AutoCreateShader    (bool mvp=true                                                          ///<自动创建着色程序
    #ifdef _DEBUG
                                                                    ,const os_char *filename=nullptr
    #endif//_DEBUG
            )=0;
		};//class Renderable
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_RENDERABLE_INCLUDE
