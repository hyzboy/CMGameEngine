#ifndef HGL_ALGORITHM_VECTOR_MATH_MGL_INCLUDE
#define HGL_ALGORITHM_VECTOR_MATH_MGL_INCLUDE

#ifdef _MSC_VER
#pragma warning(disable:4244)           // double -> int 精度丢失警告
#endif//_MSC_VER

#include<MathGeoLib.h>

/**
 * MathGeoLib
 * Game Math and Geometry Library
 *
 * My C++ library for 3D mathematics and geometry manipulation.
 * Jukka Jylänki
 *
 * offical web: http://clb.demon.fi/MathGeoLib/nightly/
 *
 * License:
 *
 *      This library is licensed under the Apache 2 license. I am not a lawyer, but to me that
 * license means that you can use this code for any purpose, both commercial and closed source.
 * You are however restricted from claiming you wrote it yourself, and cannot hold me liable
 * for anything over this code.
 *      I acknowledge that most of the non-trivial math routines are taken off a book or a
 * research paper. In all places, I have tried to be diligent to properly attribute the original
 * source. Please contact me if you feel I have misattributed something.
 */

namespace hgl
{
    using namespace math;

    typedef float2 Vector2f;
    typedef float3 Vector3f;
    typedef float4 Vector4f;

    typedef float3x3 Matrix3f;
    typedef float4x4 Matrix4f;

    inline bool operator == (const Vector2f &lhs,const Vector2f &rhs)
    {
        if(lhs.x!=rhs.x)return(false);
        if(lhs.y!=rhs.y)return(false);
        return(true);
    }

    inline bool operator != (const Vector2f &lhs,const Vector2f &rhs)
    {
        if(lhs.x!=rhs.x)return(true);
        if(lhs.y!=rhs.y)return(true);
        return(false);
    }

    inline bool operator == (const Vector3f &lhs,const Vector3f &rhs)
    {
        if(lhs.x!=rhs.x)return(false);
        if(lhs.y!=rhs.y)return(false);
        if(lhs.z!=rhs.z)return(false);
        return(true);
    }

    inline bool operator != (const Vector3f &lhs,const Vector3f &rhs)
    {
        if(lhs.x!=rhs.x)return(true);
        if(lhs.y!=rhs.y)return(true);
        if(lhs.z!=rhs.z)return(true);
        return(false);
    }

    inline bool operator == (const Vector4f &lhs,const Vector4f &rhs)
    {
        if(lhs.x!=rhs.x)return(false);
        if(lhs.y!=rhs.y)return(false);
        if(lhs.z!=rhs.z)return(false);
        if(lhs.w!=rhs.w)return(false);
        return(true);
    }

    inline bool operator != (const Vector4f &lhs,const Vector4f &rhs)
    {
        if(lhs.x!=rhs.x)return(true);
        if(lhs.y!=rhs.y)return(true);
        if(lhs.z!=rhs.z)return(true);
        if(lhs.w!=rhs.w)return(true);
        return(false);
    }

    inline void vec3to2(Vector2f &dst,const Vector3f &src)
    {
        dst.x=src.x;
        dst.y=src.y;
    }

    inline Vector2f vec3to2(const Vector3f &src)
    {
        return Vector2f(src.x,src.y);
    }

    inline void vec2to3(Vector3f &dst,const Vector2f &src,const float z)
    {
        dst.x=src.x;
        dst.y=src.y;
        dst.z=z;
    }

    inline Vector3f vec2to3(const Vector2f &src,const float z)
    {
        return Vector3f(src.x,src.y,z);
    }

    inline Matrix4f identity()
    {
        return Matrix4f::identity;
    }

    inline Matrix4f inverse(const Matrix4f &m)
    {
        return m.Inverted();
    }

    //inline Matrix4f perspective_yfov(float fovy,float aspect,float znear,float zfar);

    inline Matrix4f perspective_wh(float width,float height,float znear,float zfar)
    {
        return Matrix4f::OpenGLPerspProjRH(znear,zfar,width,height);
    }

    inline Matrix4f ortho2d(float width,float height,float znear=0,float zfar=1)
    {
        //MathGeoLib生成的2D正交矩阵中心是0,0，所以需要偏移

        return Matrix4f::OpenGLOrthoProjRH(znear,zfar,width,height)*Matrix4f::Scale(1,-1,1)*Matrix4f::Translate(-(width/2.0f),-(height/2.0f),0);
    }

    //inline Matrix4f ortho4(float left,float right,float bottom,float top,float znear=0,float zfar=1);

    inline Matrix4f lookAt(const Vector3f &eye,const Vector3f &target,const Vector3f &local_forward,const Vector3f &local_up,const Vector3f &world_up)
    {
        return Matrix4f::LookAt(eye,target,local_forward,local_up,world_up);
    }

    inline Matrix4f translate(const Vector3f &v)
    {
        return Matrix4f::Translate(v);
    }

    inline Matrix4f translate(float x,float y,float z)
    {
        return Matrix4f::Translate(x,y,z);
    }

    inline Matrix4f scale(const Vector3f &v)
    {
        return Matrix4f::Scale(v,Vector3f::zero);
    }

    inline Matrix4f scale(float x,float y,float z)
    {
        return Matrix4f::Scale(Vector3f(x,y,z),Vector3f::zero);
    }

    inline Matrix4f scale(float s)
    {
        return Matrix4f::Scale(Vector3f(s,s,s),Vector3f::zero);
    }

    inline Matrix4f rotate(float angle,const Vector3f &axis)
    {
        return Matrix4f::RotateAxisAngle(axis.Normalized(),angle);
    }

    inline Matrix4f rotate(float angle,float x,float y,float z)
    {
        return rotate(angle,Vector3f(x,y,z));
    }

    inline Matrix4f rotate(float angle,const Vector4f &axis)
    {
        return rotate(angle,Vector3f(axis.x,axis.y,axis.z));
    }

    inline Vector3f rotate(const Vector3f &v3f,float angle,const Vector3f &axis)
    {
        Vector4f result=rotate(angle,axis)*Vector4f(v3f,1.0f);

        return result.xyz();
    }

    template<typename T>
    inline T normalized(const T &v)
    {
        return v.Normalized();
    }

    template<typename T>
    inline void normalize(T &v)
    {
        v.Normalize();
    }

    template<typename T1,typename T2>
    inline Vector3f cross(const T1 &v1,const T2 &v2)
    {
        return v1.Cross(v2);
    }

    template<typename T>
    inline float dot(const T &v1,const T &v2)
    {
        return v1.Dot(v2);
    }

    inline float ray_angle_cos(const Ray &ray,const vec &pos)
    {
        return ray.dir.Dot((pos-ray.pos).Normalized());
    }
}//namespace hgl
#endif//HGL_ALGORITHM_VECTOR_MATH_MGL_INCLUDE
