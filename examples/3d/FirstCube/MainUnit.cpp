#include<hgl/Graphics.h>				//GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>			//SetClearColor,ClearScreen
#include<hgl/graph/Material.h>			//Material
#include<hgl/graph/InlineRenderable.h>	//CreateRenderableCube
#include<hgl/graph/Renderable.h>		//Renderable
#include<hgl/graph/Camera.h>			//WalkerCamera
#include<hgl/object/FlowObject.h>		//FlowObject

using namespace hgl;
using namespace hgl::graph;

const Vector3f	eye(100,70,80),
				center(0,0,0),
                up_vector(0,0,1),
                forward_vector(0,1,0);

class TestObject:public FlowObject
{
	Renderable *cube1,*cube2;

	Camera cam;

	Matrix4f proj;
	Matrix4f look;
	Matrix4f ltw1,ltw2;

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
        cam.local_forward_vector=forward_vector;
        cam.local_up_vector=up_vector;
        cam.world_up_vector=up_vector;
	}

	Renderable *CreateCube(float r,float g,float b)
	{
		Renderable *obj=CreateRenderableCube();

		//创建材质
		{
			Material *mtl=obj->GetMaterial();

			mtl->SetColorMaterial(true);		//使用Material中的颜色
			mtl->SetColor(r,g,b,1.0);
		}

		return(obj);
	}

	void CreateDualCube()
	{
		cube1=CreateCube(1,0,0);
		cube2=CreateCube(0,0,1);

#ifdef _DEBUG	//debug模式下将shader保存成文件
		cube1->AutoCreateShader(true,OS_TEXT("Cube1"));
		cube2->AutoCreateShader(true,OS_TEXT("Cube2"));
#else
		cube1->AutoCreateShader();		//默认参数是true,true
		cube2->AutoCreateShader();
#endif//_DEBUG
	}

public:

	TestObject()
	{
		SetClearColor(0.7f,0.7f,0.9f);

		CreateDualCube();
		SetCamera();

		MakeCameraMatrix(&proj,&look,&cam);

		ltw1=scale(20,20,20);
		ltw2=translate(0,0,-30)*scale(30,30,30);
	}

	~TestObject()
	{
		delete cube1;
		delete cube2;
	}

	void Draw()
	{
		ClearScreen();

		DirectRender(cube1,&proj,&look,&ltw1);
		DirectRender(cube2,&proj,&look,&ltw2);
	}
};//class TestObject

HGL_GRAPHICS_APPLICATION("立方体","Cube",new TestObject());
