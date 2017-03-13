#include"SpiralSphere.h"
#include<hgl/graph/InlineRenderable.h>

SpiralSphere::SpiralSphere(const Vector3f &up_vector)
{
    for(int i=0;i<SPHERE_LOD_LEVEL;i++)
        sphere_data[i]=CreateRenderableSphere(i+4);//false,vbtDiffuseTexCoord);                ///<创建一个球体的顶点数据，并指定纹理坐标写在那一个位置上

    for(int i=0;i<SPHERE_NUMBER;i++)
    {
        sphere_mtl[i]=new Material;

        sphere_mtl[i]->SetColorMaterial(true);
        sphere_mtl[i]->SetColor(float(i)/float(SPHERE_NUMBER),
                                1.0f,
                                0.0f,
                                1.0f);
        sphere_mtl[i]->SetDrawMode(GL_FRONT_AND_BACK,GL_FILL);

        sphere_mtl[i]->SetLightMode(HGL_VERTEX_LIGHT);                          ///<使用顶点级别的光照

//        sphere_mtl[i]->SetTexture(mtcDiffuse,GrayWhiteGrid);

        sphere_obj[i]=new Renderable(sphere_data[i/(SPHERE_NUMBER/SPHERE_LOD_LEVEL)],sphere_mtl[i]);                    ///<多个可渲染对像使用同一个顶点数据
//        sphere_obj[i]->SetTexCoord(mtcDiffuse,vbtDiffuseTexCoord);            ///<设定指定通道使用的纹理坐标数据

        sphere_obj[i]->AutoCreateShader(true,nullptr,OS_TEXT("Sphere"));

        node.AddSubNode(sphere_obj[i],  rotate(float(i)/5.0f,up_vector)*
                                        translate(float(i)/4.0f,0,0)*
                                        scale(float(i+1)/100.0f));
    }

    node.RefreshMatrix();                                                    ///<刷新当前节点下所有子节点的矩阵
    node.ExpendToList(&sphere_rl);                                            ///<展开当前节点成一个渲染列表
}

SpiralSphere::~SpiralSphere()
{
    for(int i=0;i<SPHERE_NUMBER;i++)
    {
        delete sphere_obj[i];
        delete sphere_mtl[i];
    }

    for(int i=0;i<SPHERE_LOD_LEVEL;i++)
        delete sphere_data[i];
}

void SpiralSphere::Render(const Matrix4f *proj,const Matrix4f *mv)
{
    hgl::graph::Render(&sphere_rl,proj,mv);            //渲染球列表
}
