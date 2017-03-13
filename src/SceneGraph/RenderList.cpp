#include<hgl/graph/Camera.h>
#include<hgl/graph/SceneNode.h>
#include<hgl/graph/Renderable.h>
#include<hgl/graph/VertexBuffer.h>
#include<hgl/graph/Render.h>
//#include<hgl/graph/Frustum.h>

#include<hgl/VectorMath.h>

namespace hgl
{
    namespace graph
    {
/*        float CameraLengthComp(Camera *cam,SceneNode *obj_one,SceneNode *obj_two)
        {
            if(!cam||!obj_one||!obj_two)
                return(0);

            return(    length_squared(obj_one->GetCenter(),cam->eye)-
                    length_squared(obj_two->GetCenter(),cam->eye));
        }

        bool FrustumClipFilter(const SceneNode *node,void *fc)
        {
            if(!node||!fc)return(false);

            return (((Frustum *)fc)->BoxIn(node->GetWorldBoundingBox())!=Frustum::OUTSIDE);
        }*/

        /**
        * 使用指定矩阵渲染一个渲染列表
        * @param rl 渲染列表
        * @param proj 透视矩阵
        * @param mv 视图矩阵
        * @return 是否渲染成功
        */
        bool Render(const RenderList *rl,const Matrix4f *proj,const Matrix4f *mv)
        {
            if(!rl)
                return(false);

            int count=rl->GetCount();
            const SceneNode **node=rl->GetData();

            for(int i=0;i<count;i++)
            {
                const Matrix4f fin_mv=(*mv)*(*node)->GetLocalToWorldMatrix();

                int sn=(*node)->SubData.GetCount();
                Renderable **p=(*node)->SubData.GetData();

                for(int j=0;j<sn;j++)
                {
                    DirectRender(    (*p),
                                    proj,
                                    &fin_mv);

                    p++;
                }

                node++;
            }

            return(true);
        }

        /**
        * 渲染一个渲染列表
        * @param rl 渲染列表
        * @param cam 摄像机
        * @return 是否渲染成功
        */
        bool Render(const RenderList *rl,const Camera *cam)
        {
            if(!rl||!cam)return(false);

            Matrix4f proj,mv;

            MakeCameraMatrix(&proj,&mv,cam);

            return Render(rl,&proj,&mv);
        }
    }//namespace graph
}//namespace hgl
