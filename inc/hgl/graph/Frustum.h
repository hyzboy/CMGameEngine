#ifndef HGL_GRAPH_FRUSTUM_INCLUDE
#define HGL_GRAPH_FRUSTUM_INCLUDE

#include<hgl/graph/AABox.h>
#include<hgl/graph/ClipPlane.h>
namespace hgl
{
    namespace graph
    {
        /**
        * 平截头裁剪处理
        */
        class Frustum
        {
            ClipPlane pl[6];

        public:

            enum
            {
                OUTSIDE,    //外部
                INTERSECT,  //交错
                INSIDE      //内部
            };

        public:

            Frustum();

            void SetMatrix(const Matrix4f &,const Matrix4f &);

            int PointIn(const Vector3f &)const;
            int SphereIn(const Vector3f &,float radius)const;
            int BoxIn(const AABox &)const;
        };//class Frustum
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_FRUSTUM_INCLUDE
