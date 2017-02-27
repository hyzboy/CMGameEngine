#include<hgl/Graphics.h>                //GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>            //SetClearColor,ClearColorDepthBuffer
#include<hgl/graph/Camera.h>            //WalkerCamera
#include<hgl/object/FlowObject.h>        //FlowObject

#include"PlaneGrid.h"
#include"SpiralCube.h"

using namespace hgl;
using namespace hgl::graph;

const Vector3f  eye(100,100,80),
                center(0,0,0),
                up_vector(0,0,1);

class TestObject:public FlowObject
{
    PlaneGrid *grid;
    SpiralCube *sc;

    Camera cam;

    Matrix4f proj;
    Matrix4f mv;

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

        sc=new SpiralCube(up_vector);
    }

    ~TestObject()
    {
        delete sc;
        delete grid;
    }

    void Draw(const Matrix4f *)
    {
        ClearColorDepthBuffer();

        grid->Render(&proj,&mv);

        for(int i=0;i<CUBE_NUMBER;i++)
            DirectRender(sc->cube_obj[i],&proj,&mv,&sc->cube_matrix[i]);
    }
};//class TestObject

HGL_GRAPHICS_APPLICATION("螺旋立方体排列","SpiralCube",new TestObject());
