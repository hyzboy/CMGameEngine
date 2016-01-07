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

		public:

			Renderable();					//请通过CreateRenderable来创建可渲染对像,函数在Render.h中定义
			virtual ~Renderable();

		public:

			virtual void						SetPrimitive		(uint dp)					{prim=dp;}									///<设置的绘制的图元类型
			virtual uint						GetPrimitive		()const						{return prim;}								///<取得要绘制的图元类型

			virtual bool						SetMaterial			(Material *,bool);														///<设置材质数据
			virtual Material * 					GetMaterial			()const						{return material;}							///<取得材质指针


			virtual bool						SetDrawCount		(int,int);																///<设置要绘制的数据数量
			virtual bool		                GetDrawCount		(int &,int &);															///<取得指定的要绘制的数据数量

			virtual bool						SetTexCoord			(int mtc,VertexBufferType);												///<设定贴图坐标对应缓冲区
					bool						SetTexCoord			(int mtc,VertexBufferType vbt,VertexBufferBase *vb)						///<设定贴图坐标对应缓冲区
			{
				if(!SetTexCoord(mtc,vbt))return(false);

				return SetVertexBuffer(vbt,vb);
			}

			virtual VertexBufferBase *			GetTexCoord			(int mtc,VertexBufferType *vbt=0);										///<取得贴图坐标对应的缓冲区
		};//class Renderable
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_RENDERABLE_INCLUDE
