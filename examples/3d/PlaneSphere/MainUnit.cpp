#include<hgl/Graphics.h>                //GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>            //SetClearColor,ClearColorDepthBuffer
#include<hgl/graph/Camera.h>            //WalkerCamera
#include<hgl/object/FlowObject.h>       //FlowObject
#include<hgl/graph/InlineRenderable.h>
#include<hgl/Time.h>

using namespace hgl;
using namespace hgl::graph;

const LightMode light_mode=HGL_VERTEX_LIGHT;     //光照计算模式

const Vector3f  eye(100,70,80),
                center(0,0,0),
                up_vector(0,0,1);

class TestObject:public FlowObject
{
    VertexArray *plane_va;
    Material *plane_mtl;
    Renderable *plane_obj;
    Matrix4f plane_mat;

    VertexArray *cube_va;
    Material *cube_mtl;
    Renderable *cube_obj;
    Matrix4f cube_mat;

    VertexArray *sphere_va;
    Material *sphere_mtl;
    Renderable *sphere_obj;
    Matrix4f sphere_mat;

    Camera cam;

    Matrix4f proj;
    Matrix4f mv;

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
        cam.world_up_vector=up_vector;
    }

public:

    void CreatePlane(float w,float h,float r, float g, float b)
    {
        plane_va = CreateRenderablePlane(   //创建一个平面,平面以XY轴中心点展开，Z值为0。尺寸为1x1
            true,                           //使用法线数据
            vbtNone);                       //纹理坐标对应的顶点缓冲区

        //创建材质数据
        {
            plane_mtl = new Material;
            plane_mtl->SetColorMaterial(true);
            plane_mtl->SetColor(r,g,b,1);
            plane_mtl->SetLightMode(light_mode);
        }

        //创建可渲染对象
        plane_obj = new Renderable(plane_va, plane_mtl);

#ifdef _DEBUG    //debug模式下将shader保存成文件
        plane_obj->AutoCreateShader(true, nullptr, OS_TEXT("Plane"));   ///<自动创建shader
#else
        plane_obj->AutoCreateShader();                                  ///<自动创建shader
#endif//_DEBUG

        plane_mat = scale(w,h, 1);                                      ///<平面尺寸1x1太小，放大到w*h
    }

    void CreateCube(float size, float r, float g, float b)
    {
        cube_va=CreateRenderableCube(true);                             ///<创建一个Cube的顶点数据

        //创建材质
        cube_mtl = new Material;
        cube_mtl->SetColorMaterial(true);                               ///<设定使用材质中的颜色
        cube_mtl->SetColor(r, g, b, 1.0);                               ///<设定材质颜色
        cube_mtl->SetLightMode(light_mode);

        cube_obj=new Renderable(cube_va, cube_mtl);                     ///<创建可渲染对象

#ifdef _DEBUG    //debug模式下将shader保存成文件
        cube_obj->AutoCreateShader(true, nullptr, OS_TEXT("Cube"));     ///<自动生成Cube1渲染所需的shader
#else
        cube_obj->AutoCreateShader();                                   ///<默认参数是true,nullptr
#endif//_DEBUG

        cube_mat=scale(size,size,size);
    }

    void CreateSphere(const Vector3f &center, float radius, float r, float g, float b)
    {
        sphere_va = CreateRenderableSphere(16);

        sphere_mtl = new Material;

        sphere_mtl->SetColorMaterial(true);
        sphere_mtl->SetColor(r,g,b,1.0f);
        sphere_mtl->SetLightMode(light_mode);

        sphere_obj = new Renderable(sphere_va,sphere_mtl);

        sphere_obj->AutoCreateShader(true, nullptr, OS_TEXT("Sphere"));

        sphere_mat=translate(center)*scale(radius,radius,radius);
    }

    TestObject()
    {
        SetClearColor(0,0,0);

        SetCamera();

        start_time=GetDoubleTime();

        CreatePlane(100,100,0.5,0.5,0.5);
        CreateCube(25,61.0f/255,174.0f/255.0f,233.0f/255.0f);
        CreateSphere(Vector3f(0,0,30),20,0.5,0.8,0.5);
    }

    ~TestObject()
    {
        delete cube_obj;
        delete plane_obj;
    }

    void Draw(const Matrix4f *)
    {
        ClearColorDepthBuffer();

        const double gap_time=GetDoubleTime()-start_time;

        cam.eye=rotate(eye,gap_time,up_vector);

        MakeCameraMatrix(&proj,&mv,&cam);

        DirectRender(plane_obj, proj, mv, plane_mat);
        DirectRender(cube_obj,proj,mv,cube_mat);
        DirectRender(sphere_obj,proj,mv,sphere_mat);
    }
};//class TestObject

HGL_GRAPHICS_APPLICATION("平面球体","PlaneSphere",new TestObject());
