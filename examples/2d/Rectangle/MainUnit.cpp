#include<hgl/Graphics.h>                //GraphicsApplication,SystemInitInfo
#include<hgl/graph/Bitmap.h>			//Bitmap2D
#include<hgl/graph/Texture2D.h>			//Texture2D
#include<hgl/graph/Render.h>            //SetClearColor,ClearColorDepthBuffer
#include<hgl/graph/Shader.h>            //Shader
#include<hgl/object/FlowObject.h>       //FlowObject

using namespace hgl;
using namespace hgl::graph;

//#define USE_PRIM_RECTANGLE			//不支持

#ifndef USE_PRIM_RECTANGLE
const uint16	vertex[]={	100,100,	//left-top
							356,100,	//right-top
							356,356,	//right-bottom
							100,356};	//left-bottom

const float		texcoord[]={0,0,
							1,0,
							1,1,
							0,1};
#else
const uint16	vertex[]={	100,100,	//left,top
							256,256};	//width,height

const float		texcoord[]={0,0,		//left,top
							1,1};		//width,height
#endif//USE_PRIM_RECTANGLE

class TestObject:public FlowObject
{
	Texture2D *tex;
	Material *mtl;

    VertexArray *vertex_data;		///<矩形顶点数据
    Renderable *rect_obj;			///<矩形可渲染对象

public:

    TestObject()
    {
        SetClearColor(ceMozillaCharcoal);		//背景色使用木炭黑

        //创建矩形顶点数据
        {
		#ifndef USE_PRIM_RECTANGLE			
			vertex_data=new VertexArray(HGL_PRIM_TRIANGLE_FAN);						///<创建新的顶点数据对象，图元类型为扇形

            vertex_data->SetVertex(new VB2u16(4,vertex));							///<设定矩形的坐标数据

			vertex_data->SetVertexBuffer(vbtDiffuseTexCoord,new VB2f(4,texcoord));	///<设定矩形的贴图坐标数据
		#else
			vertex_data=new VertexArray(HGL_PRIM_RECTANGLE);						///<创建新的顶点数据对象，图元类型为矩形

            vertex_data->SetVertex(new VB4u16(1,vertex));							///<设定矩形的坐标数据

			vertex_data->SetVertexBuffer(vbtDiffuseTexCoord,new VB4f(1,texcoord));	///<设定矩形的贴图坐标数据
		#endif//USE_PRIM_RECTANGLE
        }

		//加载纹理
		{
			tex=CreateTexture2D(OS_TEXT("lena_256.Tex2D"));

			mtl=new Material;
			mtl->SetTexture(mtcDiffuse,tex);
		}

		//创建渲染对象
        {
            rect_obj=new Renderable(vertex_data,mtl);						///<创建可渲染对象

			rect_obj->SetTexCoord(mtcDiffuse,vbtDiffuseTexCoord);			///<设定指定通道使用的纹理坐标数据

#ifdef _DEBUG
            rect_obj->AutoCreateShader(true,nullptr,OS_TEXT("rectangle"));	///<自动创建shader，DEBUG模式下如果是新生成shader，输出成指定文件名的文件
#else
            rect_obj->AutoCreateShader();									///<自动创建shader,正常模式下无参数
#endif//_DEBUG
        }
    }

    ~TestObject()
    {
        delete rect_obj;
        delete vertex_data;
		delete mtl;
    }

    void Draw(const Matrix4f *)
    {
        ClearColorDepthBuffer();

        DirectRender2D(rect_obj);
    }
};//class TestObject

HGL_GRAPHICS_APPLICATION("矩形","Rectangle",new TestObject());
