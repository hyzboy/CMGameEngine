#ifndef HGL_EXAMPLE_3D_SPIRAL_CUBE_INCLUDE
#define HGL_EXAMPLE_3D_SPIRAL_CUBE_INCLUDE

#include<hgl/algorithm/VectorMath.h>

#define CUBE_NUMBER	360

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

class SpiralCube
{
	VertexArray *cube_data;
	Material *cube_mtl[CUBE_NUMBER];

public:

	Renderable *cube_obj[CUBE_NUMBER];
	Matrix4f cube_matrix[CUBE_NUMBER];

public:

	SpiralCube(const Vector3f &);
	~SpiralCube();
};//class SpiralCube
#endif//HGL_EXAMPLE_3D_SPIRAL_CUBE_INCLUDE
