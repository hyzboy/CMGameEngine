#ifndef HGL_EXAMPLE_3D_SPIRAL_SPHERE_INCLUDE
#define HGL_EXAMPLE_3D_SPIRAL_SPHERE_INCLUDE

#include<hgl/algorithm/VectorMath.h>
#include<hgl/graph/SceneNode.h>

#define SPHERE_NUMBER		360
#define SPHERE_LOD_LEVEL	12		//分成12级

namespace hgl
{
	namespace graph
	{
		class VertexArray;
		class Material;
		class Renderable;
	}//namespace graph
}//namespace hgl

using namespace hgl;
using namespace hgl::graph;

class SpiralSphere
{
	VertexArray *sphere_data[SPHERE_LOD_LEVEL];
	Material *sphere_mtl[SPHERE_NUMBER];

	Renderable *sphere_obj[SPHERE_NUMBER];

protected:

	SceneNode node;					///<球体场景节点

	RenderList sphere_rl;			///<球体场景渲染列表

public:

	SpiralSphere(const Vector3f &);
	~SpiralSphere();

	void Render(const Matrix4f *,const Matrix4f *);
};//class SpiralSphere
#endif//HGL_EXAMPLE_3D_SPIRAL_SPHERE_INCLUDE
