#include<hgl/Graphics.h>                //GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>            //SetClearColor,ClearColorDepthBuffer
#include<hgl/graph/Material.h>          //Material
#include<hgl/graph/InlineRenderable.h>  //CreateRenderableCube
#include<hgl/graph/Renderable.h>        //Renderable
#include<hgl/graph/Camera.h>            //WalkerCamera
#include<hgl/object/FlowObject.h>       //FlowObject

using namespace hgl;
using namespace hgl::graph;

const Vector3f  eye(100,70,80),
                center(0,0,0),
                up_vector(0,0,1),
                forward_vector(0,1,0);

class TestObject:public FlowObject
{
    VertexArray *cube_data;                ///<立方体顶点数据
    Material *mtl1,*mtl2;                ///<两个材质
    Renderable *cube1,*cube2;            ///<两个渲染对象

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

    Renderable *CreateCube(Material *mtl,float r,float g,float b,Texture2D *tex)
    {
        //创建材质
        mtl->SetTexture(mtcDiffuse,tex);

        Renderable *obj=new Renderable(cube_data,mtl);        ///<两个可渲染对像使用同一个顶点数据

        obj->SetTexCoord(mtcDiffuse,vbtDiffuseTexCoord);    ///<设定指定通道使用的纹理坐标数据

        return obj;
    }

    void CreateDualCube()
    {
        cube_data=CreateRenderableCube(false,vbtDiffuseTexCoord);                ///<创建一个Cube的顶点数据，并指定纹理坐标写在那一个位置上

        cube1=CreateCube(mtl1=new Material,1,0,0,GrayWhiteGrid);                ///<使用内置灰白格子纹理
        cube2=CreateCube(mtl2=new Material,0,0,1,BlueWhiteGrid);                ///<使用内置蓝白格子纹理

#ifdef _DEBUG    //debug模式下将shader保存成文件
        cube1->AutoCreateShader(true,nullptr,OS_TEXT("Cube1"));                    ///<自动生成Cube1渲染所需的shader
        cube2->AutoCreateShader(true,nullptr,OS_TEXT("Cube2"));                    ///<自动生成Cube1渲染所需的shader，由于使用的纹理格式不同，产生的shader也会不同
#else
        cube1->AutoCreateShader();        //默认参数是true,nullptr
        cube2->AutoCreateShader();
#endif//_DEBUG

    }

public:

    TestObject()
    {
        SetClearColor(0.2f,0.2f,0.2f);

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

        delete cube_data;

        delete mtl1;
        delete mtl2;
    }

    void Draw(const Matrix4f *)
    {
        ClearColorDepthBuffer();

        DirectRender(cube1,&proj,&look,&ltw1);
        DirectRender(cube2,&proj,&look,&ltw2);
    }
};//class TestObject

HGL_GRAPHICS_APPLICATION("带纹理的立方体","TextureCube",new TestObject());
