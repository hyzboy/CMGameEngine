#ifndef HGL_GRAPH_AABOX_INCLUDE
#define HGL_GRAPH_AABOX_INCLUDE

#include<hgl/VectorMath.h>
namespace hgl
{
    namespace graph
    {
        /**
        * AABox
        */
        struct AABox
        {
            Vector3f corner;
            Vector3f center;
            Vector3f length;
            Vector3f corner_max;

        public:

            AABox();

            void SetCornerLength(const Vector3f &c,float x,float y,float z)     ///<按顶角和长度设置盒子范围
            {
                corner=c;
                length=Vector3f(x,y,z);
                corner_max=Vector3f(c[0]+x,c[1]+y,c[2]+z);
                center=(corner+corner_max)/2.0f;
            }

            void SetCornerLength(const Vector3f &c,const Vector3f &l)           ///<按顶角和长度设置盒子范围
            {
                SetCornerLength(c,l[0],l[1],l[2]);
            }

            void SetMinMax(const Vector3f &min_v,const Vector3f &max_v)         ///<按最小最大值设置盒子范围
            {
                corner=min_v;
                corner_max=max_v;
                length=max_v-min_v;
                center=(min_v+max_v)/2.0f;
            }

            Vector3f GetVertexP(const Vector3f &)const;
            Vector3f GetVertexN(const Vector3f &)const;

        public:

            void operator += (const AABox &);                                   ///<融合另一个AABox
        };//struct AABox
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_AABOX_INCLUDE
