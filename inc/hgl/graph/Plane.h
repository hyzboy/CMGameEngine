#ifndef HGL_GRAPH_PLANE_INCLUDE
#define HGL_GRAPH_PLANE_INCLUDE

#include<hgl/VectorMath.h>
namespace hgl
{
    namespace graph
    {
        /**
        * 面数据结构
        */
        struct Plane
        {
            Vector3f normal;
            float d;

        public:

            void Set(const Vector3f &_p,float _d)
            {
                normal=_p;
                d=_d;
            }

            void Set(const float *p)
            {
                normal.set(p[0],p[1],p[2]);
                d=p[3];
            }

            float Distance(const Vector3f &p)const
            {
                return(d+dot(normal,p));
            }
        };//struct Plane
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_PLANE_INCLUDE
