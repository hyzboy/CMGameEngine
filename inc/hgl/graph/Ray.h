#ifndef HGL_GRAPH_RAY_INCLUDE
#define HGL_GRAPH_RAY_INCLUDE

#include<hgl/VectorMath.h>
#include<hgl/graph/AABox.h>
namespace hgl
{
    namespace graph
    {
        /**
        * 射线类
        */
        struct Ray
        {
            Vector3f origin;
            Vector3f direction;

        public:

            Ray()
            {
                origin.set(0.0f,0.0f,0.0f);
                direction.set(0.0f,0.0f,0.0f);
            }

            Ray(const Vector3f &o,const Vector3f &d)
            {
                origin=o;
                direction=d;
            }

            void Set(int,int,const Matrix4f &,const Matrix4f &,const Matrix4f &);

            /**
            * 求射线上距离指定点最近的点
            * @param point 指定点
            * @return 位于射线上的点
            */
            Vector3f ClosestPoint(const Vector3f &point)const;

            /**
            * 求指定点与射线的距离的平方
            * @param point 指定点
            * @return 该点与射线的距离
            */
            float DistanceSquared(const Vector3f &point)const
            {
                return length_squared(point,ClosestPoint(point));
            }

            /**
            * 求指定点与射线的距离
            * @param point 指定点
            * @return 该点与射线的距离
            */
            float Distance(const Vector3f &point)const
            {
                return sqrt(DistanceSquared(point));
            }

            /**
             * 取得与原点指定距离的点
             */
            Vector3f GetLengthPoint(const float len)const
            {
                return origin+direction*(len/length_2d(direction));
            }

            /**
             * 取射线与指定点的夹角cos值
             */
            float GetAngleCos(const Vector3f &point)const
            {
                return dot(direction,normalize(point-origin));
            }

            /**
             * 取射线与指定点的夹角
             */
            float GetAngle(const Vector3f &point)const
            {
                return acos(GetAngleCos(point))*180.0f/HGL_PI;
            }

            /**
            * 求指定面是否与射线交汇
            */
            bool CrossPlane(const Vector3f &,const Vector3f &,const Vector3f &,const Vector3f &)const;

            /**
            * 求指定box是否与射线交汇
            */
            bool CrossBox(const Vector3f &,const Vector3f &)const;

            /**
            * 求指定box是否与射线交汇
            */
            bool CrossBox(const AABox &box)const
            {
                return CrossBox(box.corner,box.corner_max);
            }
        };//class Ray
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_RAY_INCLUDE
