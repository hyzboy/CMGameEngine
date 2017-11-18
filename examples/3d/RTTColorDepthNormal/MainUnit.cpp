#include<hgl/Graphics.h>                //GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>            //SetClearColor,ClearColorDepthBuffer
#include<hgl/graph/Camera.h>            //WalkerCamera
#include<hgl/object/FlowObject.h>       //FlowObject
#include<hgl/graph/RenderToTexture.h>   //RenderToTextureMultiChannel
#include<hgl/graph/InlineRenderable.h>  //CreateRenderableRect
#include<hgl/Time.h>
#include"SpiralCubeScene.h"

using namespace hgl;
using namespace hgl::graph;

const Vector3f  eye(100,100,80),
                center(0,0,0),
                up_vector(0,0,1);

constexpr float TILE_SIZE=256;

class TestObject:public FlowObject
{
    SpiralCubeScene *sc;                 //Cube螺旋排列场景
    RenderToTextureMultiChannel *rtt;    //渲染到纹理对象

    struct RectRenderObject
    {
        Texture2D *     tex =nullptr;   //纹理
        VertexArray *   va  =nullptr;   //顶点数据
        Material *      mtl =nullptr;   //材质
        Renderable *    ro  =nullptr;   //渲染对象

    public:

        ~RectRenderObject()
        {
            delete ro;
            delete va;
        }
    }color,depth,normal;

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
        cam.up_vector=up_vector;

        MakeCameraMatrix(&proj,&mv,&cam);
    }

    void MakeRenderRect(RectRenderObject &rro,int index)
    {
        const uint left   =(GetScreenWidth()-TILE_SIZE*3)/2;
        const uint top    =(GetScreenHeight()-TILE_SIZE)/2;

        RectScope2f pos(left+TILE_SIZE*index,
                        top,
                        TILE_SIZE,
                        TILE_SIZE);

        RectScope2f tex_coord(0,0,1,1);

        rro.va=CreateRenderableRect(pos,vbtDiffuseTexCoord,tex_coord);

        rro.mtl=new Material;
        rro.mtl->SetTexture(mtcDiffuse,rro.tex);

        rro.ro=new Renderable(rro.va,rro.mtl);
        rro.ro->SetTexCoord(mtcDiffuse,vbtDiffuseTexCoord);            ///<设定指定通道使用的纹理坐标数据
        rro.ro->AutoCreateShader();
    }

public:

    TestObject()
    {
        SetClearColor(0.2f,0.2f,0.2f);

        SetCamera();

        sc=new SpiralCubeScene(up_vector);                            ///<创建Cube螺旋排列场景

        rtt=new RenderToTextureMultiChannel(TILE_SIZE,TILE_SIZE);     ///<创建渲染到纹理对象
        {
            depth.tex   =rtt->AddDepth(HGL_SF_DEPTH32);
            color.tex   =rtt->AddColor(HGL_SF_R3_G3_B2);
            normal.tex  =rtt->AddColor(HGL_SF_RGB8);

            rtt->BindComplete();
        }

        MakeRenderRect(depth,0);
        MakeRenderRect(color,1);
        MakeRenderRect(normal,2);

        start_time=GetDoubleTime();
    }

    ~TestObject()
    {
        delete rtt;
        delete sc;
    }

    void Draw(const Matrix4f *)
    {
        ClearColorDepthBuffer();

        rtt->Begin();
        {
            rot=mv*rotate(GetDoubleTime()-start_time,Vector3f(1,0,0));

            sc->Render(&proj,&rot);            //渲染cube列表
        }
        rtt->End();

        DirectRender2D(color.ro);
        DirectRender2D(depth.ro);
        DirectRender2D(normal.ro);
    }
};//class TestObject

HGL_GRAPHICS_APPLICATION("渲染到纹理(颜色、深度、法线)","RTTColorDepthNormal",new TestObject());
