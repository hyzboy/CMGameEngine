#include"SpiralCubeScene.h"
#include<hgl/graph/Render.h>            //SetClearColor,ClearColorDepthBuffer

SpiralCubeScene::SpiralCubeScene(const Vector3f &up_vector): SpiralCube(up_vector)
{
    for(int i=0;i<CUBE_NUMBER;i++)
        node.AddSubNode(this->cube_obj[i],    this->cube_matrix[i]);            ///<增加这个可渲染对象到场景节点中，并使用指定矩阵

    node.RefreshMatrix();                                                    ///<刷新当前节点下所有子节点的矩阵
    node.ExpendToList(&cube_rl);                                            ///<展开当前节点成一个渲染列表
}

void SpiralCubeScene::Render(const Matrix4f *proj,const Matrix4f *mv)
{
    hgl::graph::Render(&cube_rl,proj,mv);            //渲染cube列表
}
