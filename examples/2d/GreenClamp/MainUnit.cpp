#include<hgl/Graphics.h>                //GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>            //SetClearColor,ClearColorDepthBuffer
#include<hgl/graph/Shader.h>            //Shader
#include<hgl/object/FlowObject.h>        //FlowObject

using namespace hgl;
using namespace hgl::graph;

const float vertex  []={0,0,    0,720,  1280,0,     1280,720};
const float texcoord[]={0,0,    0,1,    1,0,        1,1};

Texture2D *LoadBMP(const OSString &filename)
{
    uint8 *bmp;

    int64 filesize=filesystem::LoadFileToMemory(filename,(void **)&bmp);

    if(filesize<=54)return(nullptr);        //BMP文件头就不可能小于54

    uint32 datasize=*(uint32 *)(bmp+2);
    uint32 width=*(uint32 *)(bmp+18);
    uint32 height=*(uint32 *)(bmp+22);
    uint16 bit=*(uint16 *)(bmp+28);

    TSF sfmt;

    if(bit==24)sfmt=HGL_SF_RGB8;else
    if(bit==32)sfmt=HGL_SF_RGBA8;else
        return(nullptr);

    uint8 *p=bmp+54;        //bmp是BGR排列，，，颠倒一下
    uint8 t;

    for(uint32 row=0;row<height;row++)
        for (uint32 col = 0; col < width; col++)
        {
            t=*p;
            *p=*(p+2);
            *(p+2)=t;
            p+=3;
        }

    Texture2D *tex=CreateTexture2D(width, height, bmp + 54, width*height*bit / 8, sfmt, GL_RGB8);

    delete[] bmp;
    return tex;
}

class TestObject:public FlowObject
{
    VertexArray *vertex_data;       ///<三角形顶点数据

    Texture2D *back_tex,*girl_tex;
    Material *back_mtl,*girl_mtl;  

    Renderable *background;         ///<背景
    Renderable *rectangle;          ///<三角形可渲染对象

public:

    TestObject()
    {
        Color4f clamp_color;

        clamp_color.Set4i(90,128,10,96);     //HS,HS

        SetClearColor(0,0,0);

        //创建三角形顶点数据
        {
            vertex_data=new VertexArray(HGL_PRIM_TRIANGLE_STRIP);                   ///<创建新的顶点数据对象，图元类型为三角形
            vertex_data->SetVertex(new VB2f(4,vertex));                             ///<设定4个顶点的坐标数据
            vertex_data->SetVertexBuffer(vbtDiffuseTexCoord,new VB2f(4,texcoord));  ///<设定4个顶点纹理坐标数据
        }

        //加载纹理
        {
            back_tex=LoadBMP(OSString(L"back.bmp"));
            girl_tex=LoadBMP(OSString(L"green.bmp"));
        }

        //创建材质
        {
            girl_mtl=new Material;

            girl_mtl->SetTexture(mtcDiffuse,girl_tex);
            girl_mtl->SetHSVClampColor(clamp_color);
            girl_mtl->SetDepthTest(false);

            back_mtl=new Material;
            back_mtl->SetTexture(mtcDiffuse,back_tex);
            back_mtl->SetDepthTest(false);
        }

        {
            rectangle=new Renderable(vertex_data,girl_mtl);                     ///<创建可渲染对象

            rectangle->SetTexCoord(mtcDiffuse,vbtDiffuseTexCoord);              ///<设定材质纹理与数据关联

#ifdef _DEBUG
            rectangle->AutoCreateShader(true,nullptr,OS_TEXT("girl"));          ///<自动创建shader，DEBUG模式下如果是新生成shader，输出成指定文件名的文件
#else
            rectangle->AutoCreateShader();                                      ///<自动创建shader,正常模式下无参数
#endif//_DEBUG

            background= new Renderable(vertex_data, back_mtl);                  ///<创建可渲染对象
            background->SetTexCoord(mtcDiffuse, vbtDiffuseTexCoord);            ///<设定材质纹理与数据关联

#ifdef _DEBUG
            background->AutoCreateShader(true, nullptr, OS_TEXT("rectangle"));  ///<自动创建shader，DEBUG模式下如果是新生成shader，输出成指定文件名的文件
#else
            background->AutoCreateShader();                                     ///<自动创建shader,正常模式下无参数
#endif//_DEBUG
        }
    }

    ~TestObject()
    {
        delete background;
        delete rectangle;

        delete back_mtl;
        delete girl_mtl;

        delete back_tex;
        delete girl_tex;

        delete vertex_data;
    }

    void Draw(const Matrix4f *)
    {
        ClearColorDepthBuffer();

        DirectRender2D(background);
        DirectRender2D(rectangle);
    }
};//class TestObject

HGL_GRAPHICS_APPLICATION("绿幕","GreenClamp",new TestObject());
