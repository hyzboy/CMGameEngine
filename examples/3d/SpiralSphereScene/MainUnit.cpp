#include<hgl/Graphics.h>                //GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>            //SetClearColor,ClearColorDepthBuffer
#include<hgl/graph/Camera.h>            //WalkerCamera
#include<hgl/object/FlowObject.h>        //FlowObject
#include<hgl/Time.h>
#include"PlaneGrid.h"
#include"SpiralSphere.h"

using namespace hgl;
using namespace hgl::graph;

const Vector3f  eye(100,100,80),
                center(0,0,0),
                up_vector(0,0,1);

class TestObject:public FlowObject
{
    PlaneGrid *grid;
    SpiralSphere *ss;

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
        cam.world_up_vector=up_vector;

        MakeCameraMatrix(&proj,&mv,&cam);
    }

public:

    TestObject()
    {
        SetClearColor(0.2f,0.2f,0.2f);

        grid=new PlaneGrid(120,50);

        SetCamera();

        ss=new SpiralSphere(up_vector);

        start_time=GetDoubleTime();
    }

    ~TestObject()
    {
        delete ss;
        delete grid;
    }

    void Draw(const Matrix4f *)
    {
        ClearColorDepthBuffer();

        grid->Render(&proj,&mv);

        rot=mv*rotate(GetDoubleTime()-start_time,Vector3f(1,0,0));

        ss->Render(&proj,&rot);            //渲染球体列表
    }
};//class TestObject

HGL_GRAPHICS_APPLICATION("螺旋球体排列场景","SpiralSphereScene",new TestObject());
