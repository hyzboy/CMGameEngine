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
	RenderToTextureColor *rtt_color;	//渲染到纹理对象

	VertexArray *rect_data;				//矩形顶点数据
	Material *rect_mtl;					//矩形材质
	Renderable *rect_obj;				//矩形可渲染对象

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

		rtt_color=new RenderToTextureColor(512,512,hgl::graph::HGL_SF_RGB8);	///<创建渲染到纹理对象

		{
			RectScope2f pos((GetScreenWidth()-512)/2,
							(GetScreenHeight()-512)/2,
							512,
							512);

			RectScope2f tex_coord(0,0,1,1);

			rect_data=CreateRenderableRect(pos,vbtDiffuseTexCoord,tex_coord);

			rect_mtl=new Material;
			rect_mtl->SetTexture(mtcDiffuse,rtt_color->GetTexture());

			rect_obj=new Renderable(rect_data,rect_mtl);
			rect_obj->SetTexCoord(mtcDiffuse,vbtDiffuseTexCoord);			///<设定指定通道使用的纹理坐标数据
			rect_obj->AutoCreateShader();
		}

		start_time=GetDoubleTime();
	}

	~TestObject()
	{
		delete rtt_color;
		delete rect_obj;
		delete rect_data;
		delete sc;
	}

	void Draw()
	{
		ClearScreen();

		rtt_color->Begin();
		{
			rot=mv*rotate(GetDoubleTime()-start_time,Vector3f(1,0,0));

			sc->Render(&proj,&rot);			//渲染cube列表
		}
		rtt_color->End();

		DirectRender2D(rect_obj);
	}
};//class TestObject

HGL_GRAPHICS_APPLICATION("渲染到纹理(仅颜色)","RTTCOlor",new TestObject());
