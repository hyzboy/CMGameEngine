#include<hgl/Graphics.h>				//GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>			//SetClearColor,ClearScreen
#include<hgl/graph/Camera.h>			//WalkerCamera
#include<hgl/object/FlowObject.h>		//FlowObject

#include"PlaneGrid.h"

using namespace hgl;
using namespace hgl::graph;

const Vector3f  eye(100,70,80),
				center(0,0,0),
				up_vector(0,0,1),
				forward_vector(0,1,0);

class TestObject:public FlowObject
{
	PlaneGrid *grid;

	Camera cam;

	Matrix4f proj;
	Matrix4f mv;

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

		cam.local_up_vector=Vector3f(0,1,0);
		cam.local_forward_vector=Vector3f(0,0,1);
	}

public:

	TestObject()
	{
		SetClearColor(0,0,0);

		grid=new PlaneGrid(120,50,true,true,false);

		SetCamera();

        start_time=GetDoubleTime();
	}

	~TestObject()
	{
		delete grid;
	}

	void Draw()
	{
		ClearScreen();

        const double gap_time=GetDoubleTime()-start_time;

        cam.eye.z=gap_time;

        MakeCameraMatrix(&proj,&mv,&cam);

		grid->Render(&proj,&mv);
	}
};//class TestObject

HGL_GRAPHICS_APPLICATION("平面网格","PlaneGrid",new TestObject());
