#include"SpiralCube.h"
#include<hgl/graph/InlineRenderable.h>

SpiralCube::SpiralCube(const Vector3f &up_vector)
{
    cube_data=CreateRenderableCube(true,vbtDiffuseTexCoord);                ///<创建一个Cube的顶点数据，并指定纹理坐标写在那一个位置上

    for(int i=0;i<CUBE_NUMBER;i++)
    {
        cube_mtl[i]=new Material;

        cube_mtl[i]->SetColorMaterial(true);
        cube_mtl[i]->SetColor(  float(i)/float(CUBE_NUMBER),
                                1.0f,
                                0.0f,
                                1.0f);

        cube_mtl[i]->SetTexture(mtcDiffuse,GrayWhiteGrid);
        cube_mtl[i]->SetLightMode(HGL_VERTEX_LIGHT);

        cube_obj[i]=new Renderable(cube_data,cube_mtl[i]);                  ///<两个可渲染对像使用同一个顶点数据
        cube_obj[i]->SetTexCoord(mtcDiffuse,vbtDiffuseTexCoord);            ///<设定指定通道使用的纹理坐标数据

        cube_obj[i]->AutoCreateShader(true,nullptr,OS_TEXT("Cube"));

        cube_matrix[i]= rotate(float(i)/5.0f,up_vector)*
                        translate(float(i)/4.0f,0,0)*
                        scale(float(i+1)/100.0f);
    }
}

SpiralCube::~SpiralCube()
{
    for(int i=0;i<CUBE_NUMBER;i++)
    {
        delete cube_obj[i];
        delete cube_mtl[i];
    }

    delete cube_data;
}

