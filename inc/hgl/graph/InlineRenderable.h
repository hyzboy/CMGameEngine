#ifndef HGL_GRAPH_INLINE_RENDERABLE_INCLUDE
#define HGL_GRAPH_INLINE_RENDERABLE_INCLUDE

#include<hgl/VectorMath.h>
#include<hgl/type/Color4f.h>
#include<hgl/graph/Render.h>
namespace hgl
{
	namespace graph
	{
		class VertexArray;

		//内嵌可渲染数据

//		VertexArray *CreateRenderablePoint(const Vector3f &,const Color4f &);						///<创建一个点的可渲染数据

		VertexArray *CreateRenderableLine(const Vector3f &,const Vector3f &,const Color4f &);		///<创建一条线的可渲染数据

		VertexArray *CreateRenderableAxis(const float size=1);										///<创建一个坐标轴的可渲染数据

		VertexArray *CreateRenderablePlaneGrid(	const Vector3f &,									///<创建一个平面网格的可渲染数据
												const Vector3f &,
												const Vector3f &,
												const Vector3f &,
												int,int);

		VertexArray *CreateRenderableCube(	bool use_normal=false,
											bool use_tangent=false,
											const VertexBufferType tex_coord_vbt=vbtNone,
											const float tex_scale=1.0);								///<创建一个立方体的可渲染数据,尺寸为1,中心为0,0,0

		VertexArray *CreateRenderableWireCube();													///<创建一个线框立方体的可渲染数据,尺寸为1,中心为0,0,0

		VertexArray *CreateRenderableWireRect();													///<创建一个2D线框的可渲染数据
		VertexArray *CreateRenderableRect();														///<创建一个2D矩形的可渲染数据

		VertexArray *CreateRenderableWireCircle(int=32);											///<创建一个2D空心圆
		VertexArray *CreateRenderableCircle(int=32);												///<创建一个2D实心圆
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_INLINE_RENDERABLE_INCLUDE
