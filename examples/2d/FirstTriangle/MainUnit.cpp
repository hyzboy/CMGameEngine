#include<hgl/Graphics.h>				//GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>			//SetClearColor,ClearScreen
#include<hgl/graph/Shader.h>			//Shader
#include<hgl/object/FlowObject.h>		//FlowObject

using namespace hgl;
using namespace hgl::graph;

const float vertex[]={0.0,0.0,  200,0.0,    100,100 };
const float color []={1,0,0,    0,1,0,      0,0,1   };

class TestObject:public FlowObject
{
    Renderable *triangle;
    Material *mtl;

public:

	TestObject()
	{
		SetClearColor(0,0,0);

        triangle=CreateRenderable();
        mtl=CreateMaterial();

        mtl->SetColorMaterial(false);           //不使用材质中的颜色
        triangle->SetMaterial(mtl,true);

        triangle->SetPrimitive(HGL_PRIM_TRIANGLES);

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
            triangle->SetVertex(new VB2f(3,vertex));
            triangle->SetColor(new VB3f(3,color),HGL_COLOR_RGB);
        }

        triangle->AutoCreateShader(true,OS_TEXT("FirstTriangle"));
	}

	~TestObject()
	{
        delete triangle;
	}

	void Draw()
	{
		ClearScreen();

		DirectRender2D(triangle);
	}
};//class TestObject

HGL_GRAPHICS_APPLICATION("第一个三角形","FirstTriangle",new TestObject());
