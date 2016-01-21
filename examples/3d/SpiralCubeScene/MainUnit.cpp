#include<hgl/Graphics.h>				//GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>			//SetClearColor,ClearScreen
#include<hgl/graph/Camera.h>			//WalkerCamera
#include<hgl/object/FlowObject.h>		//FlowObject
#include<hgl/graph/SceneNode.h>

#include"PlaneGrid.h"
#include"SpiralCube.h"

using namespace hgl;
using namespace hgl::graph;

#define CUBE_NUMBER	360

const Vector3f  eye(100,100,80),
				center(0,0,0),
				up_vector(0,0,1);

class TestObject:public FlowObject
{
	PlaneGrid *grid;
	SpiralCube *sc;

	SceneNode node;				///<立方体场景节点
	RenderList cube_rl;			///<立方体场景渲染列表

	Camera cam;

	Matrix4f proj;
	Matrix4f mv;
	Matrix4f rot;

	double start_time;

private:

	void SetCamera()
	{
        cam.fov=45.0f;
        cam.znear=4.0f;
        cam.zfar=1000.0f;

		cam.width=GetScreenWidth();
		cam.height=GetScreenHeight();

		cam.eye=eye;
		cam.center=center;
        cam.world_up_vector=up_vector;

        MakeCameraMatrix(&proj,&mv,&cam);
	}

public:

	TestObject()
	{
		SetClearColor(0.2f,0.2f,0.2f);

		grid=new PlaneGrid(120,50);

		SetCamera();

		sc=new SpiralCube(up_vector);

		for(int i=0;i<CUBE_NUMBER;i++)
			node.AddSubNode(sc->cube_obj[i],rotate(float(i)/5.0f,up_vector)*
											translate(float(i)/4.0f,0,0)*
											scale(float(i+1)/100.0f));			///<增加这个可渲染对象到场景节点中，并使用指定矩阵

		node.RefreshMatrix();													///<刷新当前节点下所有子节点的矩阵
		node.ExpendToList(&cube_rl);											///<展开当前节点成一个渲染列表

		start_time=GetDoubleTime();
	}

	~TestObject()
	{
		delete sc;
		delete grid;
	}

	void Draw()
	{
		ClearScreen();

		grid->Render(&proj,&mv);

		rot=mv*rotate(GetDoubleTime()-start_time,Vector3f(1,0,0));

		Render(&cube_rl,&proj,&rot);			//渲染cube列表
	}
};//class TestObject

HGL_GRAPHICS_APPLICATION("螺旋立方体排列场景","SpiralCubeScene",new TestObject());
