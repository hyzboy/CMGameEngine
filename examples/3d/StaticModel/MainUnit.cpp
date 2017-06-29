#include<hgl/Graphics.h>                //GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>            //SetClearColor,ClearColorDepthBuffer
#include<hgl/graph/InlineRenderable.h>	//CreateRenderableWireCube
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
	VertexArray *box_va;
	Material *box_mtl;
	Renderable *box_r;
	Matrix4f box_mat;

public:

	TestObject()
	{
        SetClearColor(ceMozillaCharcoal);		//设置清屏颜色

		box_va=CreateRenderableWireCube();

		box_mtl=new Material;
		box_mtl->SetColorMaterial(true);
		box_mtl->SetColor(1,1,1,1);

		box_r=new Renderable(box_va,box_mtl);
		box_r->AutoCreateShader();

		box_mat=scale(50,50,50);
	}

	~TestObject()
	{
		delete box_r;
		delete box_va;
		delete box_mtl;
	}

	void Draw(const Matrix4f *)
	{
        ClearColorDepthBuffer();

		DirectRender(box_r,&proj,&mv,&box_mat);            //渲染球列表
	}
};//class TestObject

HGL_GRAPHICS_APPLICATION("静态模型显示","StaticModel",new TestObject());
