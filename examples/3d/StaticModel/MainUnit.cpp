#include<hgl/Graphics.h>                //GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>            //SetClearColor,ClearColorDepthBuffer
#include<hgl/graph/Camera.h>            //WalkerCamera
#include<hgl/object/FlowObject.h>       //FlowObject

#include"CameraControl.h"

using namespace hgl;
using namespace hgl::graph;

const Vector3f  eye(100,100,80),
                center(0,0,0),
                up_vector(0,0,1),
                forward_vector(0,1,0);

class TestObject:public CameraControlFlowObject
{

public:

	TestObject()
	{
        SetClearColor(ceMozillaCharcoal);
	}

	~TestObject()
	{
	}

	void Draw(const Matrix4f *)
	{
        ClearColorDepthBuffer();
	}
};//class TestObject

HGL_GRAPHICS_APPLICATION("静态模型显示","StaticModel",new TestObject());
