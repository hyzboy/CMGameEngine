#include<hgl/Graphics.h>				//GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>			//SetClearColor,ClearScreen
#include<hgl/graph/Shader.h>			//Shader
#include<hgl/object/FlowObject.h>		//FlowObject
#include<hgl/graph/InlineRenderable.h>

using namespace hgl;
using namespace hgl::graph;

class TestObject:public FlowObject
{
    Renderable *triangle;
    Material *mtl;

    Renderable *axis;

    double start_time;

public:

	TestObject()
	{
		SetClearColor(0,0,0);

        axis=CreateRenderableAxis(100);
        axis->AutoCreateShader(true,OS_TEXT("Axis"));

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
            const float vertex[]={0.0,0.0,  100,0.0,    100,100 };
            const float color []={1,0,0,    0,1,0,      0,0,1   };

            triangle->SetVertex(new VB2f(3,vertex));
            triangle->SetColor(new VB3f(3,color),HGL_COLOR_RGB);
        }

        triangle->AutoCreateShader(true,OS_TEXT("FirstTriangle"));

        start_time=GetDoubleTime();
	}

	~TestObject()
	{
        delete triangle;
	}

	void Draw()
	{
		ClearScreen();

        //MathGeoLib生成的2D正交矩阵中心是0,0，所以需要偏移

        double time_gap=GetDoubleTime()-start_time;

        Matrix4f proj=Matrix4f::OpenGLOrthoProjRH(-100,100,640,480);

        //Matrix4f mv=Matrix4f::Scale(Vector3f(1,-1,1),Vector3f::zero)*Matrix4f::RotateZ(time_gap,Vector3f(0,0,1))*;

        Matrix4f mv=Matrix4f::Translate(time_gap,0,0);

        Matrix4f result=proj*mv;

//		DirectRender(triangle,&result,0);
        DirectRender(axis,&result,0);
	}
};//class TestObject

HGL_GRAPHICS_APPLICATION("第一个三角形","FirstTriangle",new TestObject());
