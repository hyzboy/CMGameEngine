#include<hgl/Graphics.h>                //GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>            //SetClearColor,ClearColorDepthBuffer
#include<hgl/graph/Shader.h>            //Shader
#include<hgl/object/FlowObject.h>        //FlowObject

using namespace hgl;
using namespace hgl::graph;

const float vertex[]={100,100,  200,100,    200,200 };
const float color []={1,0,0,    0,1,0,      0,0,1   };

class TestObject:public FlowObject
{
    VertexArray *vertex_data;        ///<三角形顶点数据
    Renderable *triangle;            ///<三角形可渲染对象

public:

    TestObject()
    {
        SetClearColor(0,0,0);

        //创建三角形顶点数据
        {
            vertex_data=new VertexArray(HGL_PRIM_TRIANGLES);            ///<创建新的顶点数据对象，图元类型为三角形

            {    //传统风格
//             VB2f *vertex=new VB2f(3);
//             VB3f *color=new VB3f(3);
//
//             vertex->Begin();color->Begin();
//                 vertex->Write(100,100);   color->Write(1,0,0);
//                 vertex->Write(200,100);   color->Write(0,1,0);
//                 vertex->Write(200,200);   color->Write(0,0,1);
//             vertex->End();color->End();
//
//             triangle->SetVertex(vertex);
//             triangle->SetColor(color,HGL_COLOR_RGB);
            }

            {    //直接传送风格，这两行与上面整段结果一致
                vertex_data->SetVertex(new VB2f(3,vertex));                ///<设定3个顶点的坐标数据
                vertex_data->SetColor(new VB3f(3,color),HGL_COLOR_RGB);    ///<设定3个顶点的颜色数据
            }
        }

        {
            triangle=new Renderable(vertex_data);                                ///<创建可渲染对象

#ifdef _DEBUG
            triangle->AutoCreateShader(true,nullptr,"triangle");                ///<自动创建shader，DEBUG模式下如果是新生成shader，输出成指定文件名的文件
#else
            triangle->AutoCreateShader();                                        ///<自动创建shader,正常模式下无参数
#endif//_DEBUG
        }
    }

    ~TestObject()
    {
        delete triangle;
        delete vertex_data;
    }

    void Draw()
    {
        ClearColorDepthBuffer();

        DirectRender2D(triangle);
    }
};//class TestObject

HGL_GRAPHICS_APPLICATION("第一个三角形","FirstTriangle",new TestObject());
