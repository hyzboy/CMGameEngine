#include<hgl/Graphics.h>                //GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>            //SetClearColor,ClearColorDepthBuffer
#include<hgl/graph/Camera.h>            //WalkerCamera
#include<hgl/object/FlowObject.h>        //FlowObject
#include<hgl/Time.h>
#include"PlaneGrid.h"
#include"SpiralSphere.h"
#include"CameraControl.h"

using namespace hgl;
using namespace hgl::graph;

const Vector3f  eye(100,100,80),
                center(0,0,0),
                up_vector(0,0,1),
                forward_vector(0,1,0);

class TestObject:public CameraControlFlowObject
{
    PlaneGrid *grid;
    SpiralSphere *ss;

public:

	TestObject()
	{
        SetClearColor(0.2f,0.2f,0.2f);

        grid=new PlaneGrid(120,50);

        ss=new SpiralSphere(up_vector);
	}

	~TestObject()
	{
        delete ss;
        delete grid;
	}

	void Draw(const Matrix4f *)
	{
        ClearColorDepthBuffer();

        grid->Render(&proj,&mv);
        
        ss->Render(&proj,&mv);            //渲染球体列表
	}
};//class TestObject

HGL_GRAPHICS_APPLICATION("摄像机控制","CameraControl",new TestObject());
