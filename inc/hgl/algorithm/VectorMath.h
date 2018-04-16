#ifndef HGL_ALGORITHM_VECTOR_MATH_INCLUDE
#define HGL_ALGORITHM_VECTOR_MATH_INCLUDE

#include<hgl/type/DataType.h>

//注：GLM/CML(OpenGLMode)是列矩阵,计算坐标matrix*pos
//   而MGL是行矩阵，需要反过来pos*matrix

#include<hgl/algorithm/MathMGL.h>        // Game Math and Geometry Library

namespace hgl
{
    namespace algorithm
    {
        double Lsin(int angle);                                     ///<低精度sin计算,注意传入的参数为角度而非弧度
        double Lcos(int angle);                                     ///<低精度cos计算,注意传入的参数为角度而非弧度
        void Lsincos(int angle, double &s, double &c);              ///<低精度sin+cos计算,注意传入的参数为角度而非弧度

        double inline Latan(double z);                              ///<低精度atan函数
        double Latan2(double y, double x);                          ///<低精度atan2函数

        inline float length_squared(const Vector2f &v)
        {
            return (v.x*v.x) + (v.y*v.y);
        }

        inline float length_squared_2d(const Vector3f &v)
        {
            return (v.x*v.x) + (v.y*v.y);
        }

        inline float length_squared(const Vector3f &v)
        {
            return (v.x*v.x) + (v.y*v.y) + (v.z*v.z);
        }

        inline float length_squared(const Vector4f &v)
        {
            return (v.x*v.x) + (v.y*v.y) + (v.z*v.z);
        }

        template<typename T>
        inline float length(const T &v)
        {
            return sqrt(length_squared(v));
        }

        inline float length_2d(const Vector3f &v)
        {
            return sqrt(length_squared_2d(v));
        }

        template<typename T1, typename T2>
        inline float length_squared(const T1 &v1, const T2 &v2)
        {
            const float x = (v1.x - v2.x);
            const float y = (v1.y - v2.y);

            return x*x + y*y;
        }

        template<typename T1, typename T2>
        inline float length(const T1 &v1, const T2 &v2)
        {
            return sqrt(length_squared(v1, v2));
        }

        inline float length_squared(const Vector3f &v1, const Vector3f &v2)
        {
            const float x = (v1.x - v2.x);
            const float y = (v1.y - v2.y);
            const float z = (v1.z - v2.z);

            return x*x + y*y + z*z;
        }

        template<typename T1, typename T2>
        inline float length_squared_2d(const T1 &v1, const T2 &v2)
        {
            const float x = (v1.x - v2.x);
            const float y = (v1.y - v2.y);

            return x*x + y*y;
        }

        inline float length(const Vector3f &v1, const Vector3f &v2)
        {
            return sqrt(length_squared(v1, v2));
        }

        template<typename T1, typename T2>
        inline float length_2d(const T1 &v1, const T2 &v2)
        {
            return sqrt(length_squared_2d(v1, v2));
        }

        inline Vector2f to(const Vector2f &start, const Vector2f &end, float pos)
        {
            return Vector2f(start.x + (end.x - start.x)*pos,
                start.y + (end.y - start.y)*pos);
        }

        inline Vector3f to(const Vector3f &start, const Vector3f &end, float pos)
        {
            return Vector3f(start.x + (end.x - start.x)*pos,
                start.y + (end.y - start.y)*pos,
                start.z + (end.z - start.z)*pos);
        }

        template<typename T>
        inline void to_2d(T &result, const T &start, const T &end, float pos)
        {
            result.x = start.x + (end.x - start.x)*pos;
            result.y = start.y + (end.y - start.y)*pos;
        }

        inline float ray_angle_cos(const Vector3f &ray_dir, const Vector3f &ray_pos, const Vector3f &pos)
        {
            return dot(ray_dir, normalized(pos - ray_pos));
        }

        /**
         * 做一个2D旋转计算
         * @param result 结果
         * @param source 原始点坐标
         * @param center 圆心坐标
         * @param ang 旋转角度
         */
        template<typename T1, typename T2, typename T3>
        inline void rotate2d(T1 &result, const T2 &source, const T3 &center, const double ang)
        {
            double as, ac;
            //        double nx,ny;

            //      as=sin(ang*(HGL_PI/180.0f));
            //      ac=cos(ang*(HGL_PI/180.0f));
                    //sincos(ang*(HGL_PI/180.0f),&as,&ac);      //在80x87指令上，sin/cos是一个指令同时得出sin和cos，所以可以这样做
            Lsincos(ang, as, ac);                         //低精度sin/cos计算

            result.x = center.x + ((source.x - center.x)*ac - (source.y - center.y)*as);
            result.y = center.y + ((source.x - center.x)*as + (source.y - center.y)*ac);
        }
    }//namespace algorithm
}//namespace hgl
#endif//HGL_ALGORITHM_VECTOR_MATH_INCLUDE
