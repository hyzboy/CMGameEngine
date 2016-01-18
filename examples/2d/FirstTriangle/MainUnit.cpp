#include<hgl/Graphics.h>				//GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>			//SetClearColor,ClearScreen
#include<hgl/graph/Shader.h>			//Shader
#include<hgl/object/FlowObject.h>		//FlowObject

using namespace hgl;
using namespace hgl::graph;

const float vertex[]={100,100,  200,100,    200,200 };
const float color []={1,0,0,    0,1,0,      0,0,1   };

class TestObject:public FlowObject
{
	VertexArray *vertex_data;		///<三角形顶点数据
    Material *material;				///<材质
    Renderable *triangle;			///<三角形可渲染对象

public:

	TestObject()
	{
		SetClearColor(0,0,0);

		//创建三角形顶点数据
		{
			vertex_data=new VertexArray(HGL_PRIM_TRIANGLES);			///<创建新的顶点数据对象，图元类型为三角形

//         {
//             VB2f *vertex=new VB2f(3);
//             VB3f *color=new VB3f(3);
//
//             vertex->Begin();color->Begin();
//                 vertex->Write(0.0,0.0);   color->Write(1,0,0);
//                 vertex->Write(100,0.0);   color->Write(0,1,0);
//                 vertex->Write(100,100);   color->Write(0,0,1);
//             vertex->End();color->End();
//
//             triangle->SetVertex(vertex);
//             triangle->SetColor(color,HGL_COLOR_RGB);
//         }

        //下面这段代码与上面这段等价
			{
				vertex_data->SetVertex(new VB2f(3,vertex));				///<设定顶点数据
				vertex_data->SetColor(new VB3f(3,color),HGL_COLOR_RGB);	///<设定颜色数据
			}
		}

		//创建材质
		{
			material=new Material();

			material->SetColorMaterial(false);           //不使用材质中的颜色
		}

		{
			triangle=new Renderable(vertex_data,material);						///<创建

#ifdef _DEBUG
			triangle->AutoCreateShader(true,nullptr,"triangle");				///<自动创建shader，DEBUG模式下如果是新生成shader，输出成指定文件名的文件
#else
			triangle->AutoCreateShader();										///<自动创建shader,正常模式下无参数
#endif//_DEBUG
		}
	}

	~TestObject()
	{
        delete triangle;
		delete material;
		delete vertex_data;
	}

	void Draw()
	{
		ClearScreen();

		DirectRender2D(triangle);
	}
};//class TestObject

HGL_GRAPHICS_APPLICATION("第一个三角形","FirstTriangle",new TestObject());
