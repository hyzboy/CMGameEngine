#include<hgl/Graphics.h>				//GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>			//SetClearColor,ClearScreen
#include<hgl/graph/Camera.h>			//WalkerCamera
#include<hgl/object/FlowObject.h>		//FlowObject

#include"PlaneGrid.h"

using namespace hgl;
using namespace hgl::graph;

const Vector3f	eye(100,70,80),
				center(0,0,0),
				up_vector(0,0,1),
				forward_vector(0,1,0);

class TestObject:public FlowObject
{
	PlaneGrid *grid;

	Camera cam;

	Matrix4f proj;
	Matrix4f look;

private:

	void SetCamera()
	{
		cam.yfov=45.0f;
		cam.znear=4.0f;
		cam.zfar=1000.0f;

		cam.width=GetScreenWidth();
		cam.height=GetScreenHeight();

		cam.eye=eye;
		cam.center=center;
		cam.local_up_vector=up_vector;
        cam.world_up_vector=up_vector;
		cam.forward_vector=forward_vector;
	}

public:

	TestObject()
	{
		SetClearColor(0,0,0);

		grid=new PlaneGrid(100,40);

		SetCamera();

		MakeCameraMatrix(&proj,&look,&cam);
	}

	~TestObject()
	{
		delete grid;
	}

	void Draw()
	{
		ClearScreen();

		grid->Render(&proj,&look);
	}
};//class TestObject

HGL_GRAPHICS_APPLICATION("平面网格","PlaneGrid",new TestObject());
