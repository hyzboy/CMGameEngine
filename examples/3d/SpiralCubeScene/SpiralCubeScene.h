#ifndef HGL_EXAMPLE_3D_SPIRAL_CUBE_SCENE_INCLUDE
#define HGL_EXAMPLE_3D_SPIRAL_CUBE_SCENE_INCLUDE

#include"SpiralCube.h"
#include<hgl/graph/SceneNode.h>

class SpiralCubeScene:public SpiralCube
{
	SceneNode node;				///<立方体场景节点
	RenderList cube_rl;			///<立方体场景渲染列表

public:

	SpiralCubeScene(const Vector3f &);

	void Render(const Matrix4f *,const Matrix4f *);
};//class SpiralCubeScene
#endif//HGL_EXAMPLE_3D_SPIRAL_CUBE_SCENE_INCLUDE
