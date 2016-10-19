#include<hgl/Graphics.h>				//GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>			//SetClearColor,ClearColorDepthBuffer
#include<hgl/graph/Camera.h>			//WalkerCamera
#include<hgl/object/FlowObject.h>		//FlowObject

#include"PlaneGrid.h"

using namespace hgl;
using namespace hgl::graph;

const Vector3f  eye(100,70,80),
				center(0,0,0),
				up_vector(0,0,1);

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
	}

public:

	TestObject()
	{
		SetClearColor(0,0,0);

		grid=new PlaneGrid(120,50);

		SetCamera();

        start_time=GetDoubleTime();
	}

	~TestObject()
	{
		delete grid;
	}

	void Draw()
	{
		ClearColorDepthBuffer();

        const double gap_time=GetDoubleTime()-start_time;

        cam.eye=rotate(eye,gap_time,up_vector);

        MakeCameraMatrix(&proj,&mv,&cam);

		grid->Render(&proj,&mv);
	}
};//class TestObject

HGL_GRAPHICS_APPLICATION("平面网格","PlaneGrid",new TestObject());
