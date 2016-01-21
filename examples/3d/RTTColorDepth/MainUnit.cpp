#include<hgl/Graphics.h>				//GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>			//SetClearColor,ClearScreen
#include<hgl/graph/Camera.h>			//WalkerCamera
#include<hgl/object/FlowObject.h>		//FlowObject
#include<hgl/graph/RenderToTexture.h>
#include<hgl/graph/InlineRenderable.h>

#include"SpiralCubeScene.h"

using namespace hgl;
using namespace hgl::graph;

#define CUBE_NUMBER	360

const Vector3f  eye(100,100,80),
				center(0,0,0),
				up_vector(0,0,1);

class TestObject:public FlowObject
{
	SpiralCubeScene *sc;				//Cube螺旋排列场景
	RenderToTextureColorDepth *rtt;		//渲染到纹理对象

	VertexArray *rect_data_color;		//矩形顶点数据
	VertexArray *rect_data_depth;		//矩形顶点数据

	Material *rect_mtl_color;			//矩形材质
	Renderable *rect_obj_color;			//矩形可渲染对象

	Material *rect_mtl_depth;			//矩形材质
	Renderable *rect_obj_depth;			//矩形可渲染对象

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

		SetCamera();

		sc=new SpiralCubeScene(up_vector);							///<创建Cube螺旋排列场景

		rtt=new RenderToTextureColorDepth(256,256,HGL_SF_RGB8,HGL_SF_DEPTH32F);	///<创建渲染到纹理对象

		{
			RectScope2f pos((GetScreenWidth()-512)/2,
							(GetScreenHeight()-256)/2,
							256,
							256);

			RectScope2f tex_coord(0,0,1,1);

			rect_data_color=CreateRenderableRect(pos,vbtDiffuseTexCoord,tex_coord);

			rect_mtl_color=new Material;
			rect_mtl_color->SetTexture(mtcDiffuse,rtt->GetColorTexture());

			rect_obj_color=new Renderable(rect_data_color,rect_mtl_color);
			rect_obj_color->SetTexCoord(mtcDiffuse,vbtDiffuseTexCoord);			///<设定指定通道使用的纹理坐标数据
			rect_obj_color->AutoCreateShader();
		}

		{
			RectScope2f pos(((GetScreenWidth()-512)/2)+256,
							(GetScreenHeight()-256)/2,
							256,
							256);

			RectScope2f tex_coord(0,0,1,1);

			rect_data_depth=CreateRenderableRect(pos,vbtDiffuseTexCoord,tex_coord);

			rect_mtl_depth=new Material;
			rect_mtl_depth->SetTexture(mtcDiffuse,rtt->GetDepthTexture());

			rect_obj_depth=new Renderable(rect_data_depth,rect_mtl_depth);
			rect_obj_depth->SetTexCoord(mtcDiffuse,vbtDiffuseTexCoord);			///<设定指定通道使用的纹理坐标数据
			rect_obj_depth->AutoCreateShader();
		}

		start_time=GetDoubleTime();
	}

	~TestObject()
	{
		delete rtt;
		delete rect_obj_color;
		delete rect_obj_depth;
		delete rect_data_color;
		delete rect_data_depth;
		delete sc;
	}

	void Draw()
	{
		ClearScreen();

		rtt->Begin();
		{
			rot=mv*rotate(GetDoubleTime()-start_time,Vector3f(1,0,0));

			sc->Render(&proj,&rot);			//渲染cube列表
		}
		rtt->End();

		DirectRender2D(rect_obj_color);
		DirectRender2D(rect_obj_depth);
	}
};//class TestObject

HGL_GRAPHICS_APPLICATION("渲染到纹理(颜色与深度)","RTTColorDepth",new TestObject());
