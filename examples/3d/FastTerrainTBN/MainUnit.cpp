/**
 * 实验项目，此实验结果并未成功
 */
#include<hgl/Graphics.h>                //GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>            //SetClearColor,ClearColorDepthBuffer
#include<hgl/graph/Material.h>          //Material
#include<hgl/graph/InlineRenderable.h>  //CreateRenderableAxis
#include<hgl/graph/Renderable.h>        //Renderable
#include<hgl/graph/Camera.h>            //WalkerCamera
#include<hgl/object/FlowObject.h>       //FlowObject
#include<hgl/Time.h>                    //GetDoubleTime

#include<time.h>

using namespace hgl;
using namespace hgl::graph;

const Vector3f  eye(12,6 ,10),
                center(0,0,0),
                up_vector(0,1,0),
                forward_vector(0,1,0);

void WriteTBN(VertexBufferBase *vbb,const Vector4f &t,const Vector4f &b,const Vector4f &n)
{
    VB3f *tbn=(VB3f *)vbb;

    tbn->Begin(0);
        tbn->Write(0,0,0);tbn->Write(t);
        tbn->Write(0,0,0);tbn->Write(b);
        tbn->Write(0,0,0);tbn->Write(n);
    tbn->End();
}

/**
 * 创建一个TBN轴的可渲染数据
 * @param size 坐标轴尺寸
 * @return 可渲染数据
 */
Renderable *CreateRenderableTBN(const Vector4f &t,const Vector4f &b,const Vector4f &n,const float light)
{
    VertexArray *obj=new VertexArray(HGL_PRIM_LINES);

    //创建顶点缓冲区
    VB3f *vertex=new VB3f(6);               //创建一个3D坐标点、浮点数据的缓冲区
    VB3f *color=new VB3f(6);                //创建一个4元颜色、浮点数据的缓冲区

    color->Begin();
    vertex->Begin();
        color->Write(light,0,0);vertex->Write(0,0,0);color->Write(light,0,0);vertex->Write(t);
        color->Write(0,light,0);vertex->Write(0,0,0);color->Write(0,light,0);vertex->Write(b);
        color->Write(0,0,light);vertex->Write(0,0,0);color->Write(0,0,light);vertex->Write(n);
    vertex->End();
    color->End();

    obj->SetVertex(vertex);
    obj->SetColor(color,HGL_PC_RGB);

    return(new Renderable(obj,nullptr));
}

class TestObject:public FlowObject
{
    Renderable *tbn[4];

    Camera cam;

    Matrix4f proj;
    Matrix4f look;
    Matrix4f ltw[4];

    double last_time;

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

    double get_angle(const Vector2f &v1,const Vector2f &v2)
    {
        double product=v1.x*v2.x+v1.y*v2.y;

        double v1_mod=sqrt(v1.x*v1.x+v1.y*v1.y);
        double v2_mod=sqrt(v2.x*v2.x+v2.y*v2.y);

        double cv=product/(v1_mod*v2_mod);

        if(cv<-1&&cv>-2)
            cv=-1;
        else
        if(cv>1&&cv<2)
            cv=1;

        return acos(cv)*180.0f/HGL_PI;
    }

    Vector2f rotate2d(const Vector2f &v,double ang)
    {
		double as,ac;
		double nx,ny;

        as=sin(ang*HGL_PI/180.0f);
		ac=cos(ang*HGL_PI/180.0f);

		nx=v.x*ac-v.y*as;
		ny=v.x*as+v.y*ac;

        return Vector2f(nx,ny);
    }

    void CreateTBN()
    {
        //缺省TBN
        Vector4f t(1,0,0,0);
        Vector4f b(0,0,1,0);
        Vector4f n(0,1,0,0);

        //原始坐标系
        tbn[0]=CreateRenderableTBN(Vector4f(1,0,0,0),Vector4f(0,1,0,0),Vector4f(0,0,1,0),1);
        tbn[0]->AutoCreateShader(true,nullptr,OS_TEXT("TBN"));
        tbn[1]=CreateRenderableTBN(t,b,n,1);
        tbn[1]->AutoCreateShader(true,nullptr,OS_TEXT("TBN"));
        tbn[2]=CreateRenderableTBN(t,b,n,0.5);
        tbn[2]->AutoCreateShader(true,nullptr,OS_TEXT("TBN"));
        tbn[3]=CreateRenderableTBN(t,b,n,0.35);
        tbn[3]->AutoCreateShader(true,nullptr,OS_TEXT("TBN"));
    }

public:

    TestObject()
    {
        ::srand(::time(0));

        SetClearColor(0.2f,0.2f,0.2f);

        CreateTBN();
        SetCamera();

        MakeCameraMatrix(&proj,&look,&cam);

        ltw[0]=scale(100,100,100);
        ltw[1]=translate(2,1,0);
        ltw[2]=translate(4,1,0);
        ltw[3]=translate(6,1,0);

        last_time=GetDoubleTime();
    }

    ~TestObject()
    {
        for(int i=0;i<4;i++)
            delete tbn[i];
    }

    void RecomputeTBN()
    {
        //缺省TBN
        Vector4f t(1,0,0,0);
        Vector4f b(0,0,1,0);
        Vector4f n(0,1,0,0);

        Matrix4f rot;

        do
        {
            rot=rotate(rand(),Vector4f(rand(),rand(),rand(),0));                                    //随机一个旋转矩阵
        
            n=normalized(n*rot);
            t=normalized(t*rot);
            b=normalized(b*rot);
        }while(n.y<=0);                                                           //地形高度图法线Y不可能为负值
        
        WriteTBN(tbn[1]->GetVertexBuffer(vbtVertex),t,b,n);

        //第二个TBN数据，N/T使用之前的正常值,B使用cross计算
        WriteTBN(tbn[2]->GetVertexBuffer(vbtVertex),t,cross(t,n),n);

        //第三个TBN数据，T/B全部使用N算出来

        //算出旋转角度
        
        Vector2f xy_0(0.0,1.0);
        Vector2f xy_n(n.x,n.y);

        double xy_ang=get_angle(xy_0,xy_n);

        Vector2f xz_0(0,1);
        Vector2f xz_n(n.x,n.z);

        double xz_ang=get_angle(xz_0,xz_n);
        
        Vector2f new_t_xy=rotate2d(Vector2f(1,0),xy_ang);
        Vector2f new_t_xz=rotate2d(Vector2f(1,0),xz_ang);
        Vector4f new_t;
        
        if(n.x<0)
            new_t=Vector4f(new_t_xy.x,new_t_xy.y,1-new_t_xz.y,0);
        else
            new_t=Vector4f(new_t_xy.x,-new_t_xy.y,1-new_t_xz.y,0);

        //Vector2f new_b_xy=rotate2d(Vector2f(0,0),xy_ang);
        //Vector2f new_b_xz=rotate2d(Vector2f(0,1),xz_ang);
        //Vector4f new_b(new_b_xy.x,new_b_xy.y,1-new_b_xz.y,0);

        new_t=normalized(new_t);
//        new_b=normalized(new_b);

        WriteTBN(tbn[3]->GetVertexBuffer(vbtVertex),new_t,cross(new_t,n),n);
    }
        
    void Draw(const Matrix4f *)
    {
        ClearColorDepthBuffer();

        for(int i=0;i<4;i++)
            DirectRender(tbn[i],&proj,&look,&ltw[i]);

        double cur_time=GetDoubleTime();

        if(cur_time-last_time>1)
        {
            RecomputeTBN();
            last_time=cur_time;
        }
    }
};//class TestObject

HGL_GRAPHICS_APPLICATION("快速地形TBN计算","FastTerrainTBN",new TestObject());
