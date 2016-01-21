#include<hgl/Graphics.h>				//GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>			//SetClearColor,ClearScreen
#include<hgl/graph/Camera.h>			//WalkerCamera
#include<hgl/object/FlowObject.h>		//FlowObject
#include<hgl/graph/InlineRenderable.h>

#include"PlaneGrid.h"

using namespace hgl;
using namespace hgl::graph;

#define CUBE_NUMBER	360

const Vector3f  eye(100,100,80),
				center(0,0,0),
				up_vector(0,0,1);

class TestObject:public FlowObject
{
	PlaneGrid *grid;

	VertexArray *cube_data;
	Material *cube_mtl[CUBE_NUMBER];
	Renderable *cube_obj[CUBE_NUMBER];
	Matrix4f cube_matrix[CUBE_NUMBER];

	Camera cam;

	Matrix4f proj;
	Matrix4f mv;

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

		{
			cube_data=CreateRenderableCube(false,false,vbtDiffuseTexCoord);			///<创建一个Cube的顶点数据，并指定纹理坐标写在那一个位置上

			for(int i=0;i<CUBE_NUMBER;i++)
			{
				cube_mtl[i]=new Material;

				cube_mtl[i]->SetColorMaterial(true);
				cube_mtl[i]->SetColor(	float(i)/float(CUBE_NUMBER),
										1.0f,
										0.0f,
										1.0f);

				cube_mtl[i]->SetTexture(mtcDiffuse,GrayWhiteGrid);

				cube_obj[i]=new Renderable(cube_data,cube_mtl[i]);					///<两个可渲染对像使用同一个顶点数据
				cube_obj[i]->SetTexCoord(mtcDiffuse,vbtDiffuseTexCoord);			///<设定指定通道使用的纹理坐标数据

				cube_obj[i]->AutoCreateShader();

				cube_matrix[i]=	rotate(float(i)/5.0f,up_vector)*
								translate(float(i)/4.0f,0,0)*
								scale(float(i+1)/100.0f);
			}
		}
	}

	~TestObject()
	{
		for(int i=0;i<CUBE_NUMBER;i++)
		{
			delete cube_obj[i];
			delete cube_mtl[i];
		}

		delete cube_data;

		delete grid;
	}

	void Draw()
	{
		ClearScreen();

		grid->Render(&proj,&mv);

		for(int i=0;i<CUBE_NUMBER;i++)
			DirectRender(cube_obj[i],&proj,&mv,&cube_matrix[i]);
	}
};//class TestObject

HGL_GRAPHICS_APPLICATION("螺旋立方体排列","SpiralCube",new TestObject());
