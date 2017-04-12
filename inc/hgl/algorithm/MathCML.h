#ifndef HGL_ALGORITHM_VECTOR_MATH_CML_INCLUDE
#define HGL_ALGORITHM_VECTOR_MATH_CML_INCLUDE

/**
 * CML
 * Configurable Math Library
 *
 * The CML (Configurable Math Library) is a free C++ math library for games and graphics.
 *
 * offical web:http://cmldev.net/
 * License: Boost Software License,v1.0 (see "doc/license/Boost Software License 1.0.txt" for details).
 */
#include<cml/cml.h>

namespace hgl
{
//  typedef cml::vector2i Vector2i;
//  typedef cml::vector3i Vector3i;
//  typedef cml::vector4i Vector4i;
//
    typedef cml::vector2f Vector2f;
    typedef cml::vector3f Vector3f;
    typedef cml::vector4f Vector4f;

    typedef cml::matrix33f_c Matrix3f;
    typedef cml::matrix44f_c Matrix4f;

    template<typename T1,typename T2>
    void vec3to2(cml::vector<T1,cml::fixed<2> > &dst,const cml::vector<T2,cml::fixed<3> > &src)
    {
        dst[0]=src[0];
        dst[1]=src[1];
    }

    template<typename T>
    cml::vector<T,cml::fixed<2> > vec3to2(const cml::vector<T,cml::fixed<3> > &src)
    {
        cml::vector<T,cml::fixed<2> > dst;

        dst[0]=src[0];
        dst[1]=src[1];

        return dst;
    }

    template<typename T1,typename T2>
    void vec2to3(cml::vector<T1,cml::fixed<3> > &dst,const cml::vector<T2,cml::fixed<2> > &src,const T1 &z)
    {
        dst[0]=src[0];
        dst[1]=src[1];
        dst[2]=z;
    }

    template<typename T>
    cml::vector<T,cml::fixed<3> > vec2to3(const cml::vector<T,cml::fixed<2> > &src,const T &z)
    {
        cml::vector<T,cml::fixed<3> > dst;

        dst[0]=src[0];
        dst[1]=src[1];
        dst[2]=z;

        return dst;
    }

    inline Matrix4f identity()
    {
        return cml::identity_4x4();
    }

    inline Matrix4f inverse(const Matrix4f &m)
    {
        return cml::inverse(m);
    }

    inline Matrix4f perspective_yfov(float fovy,float aspect,float znear,float zfar)
    {
        Matrix4f result;

        cml::matrix_perspective_yfov_RH(result,fovy,aspect,znear,zfar,cml::z_clip_zero);

        return result;
    }

    inline Matrix4f perspective_WH(float width,float height,float znear,float zfar)
    {
        Matrix4f result;

        cml::matrix_perspective_RH(result,width,height,znear,zfar,cml::z_clip_zero);
    }

    inline Matrix4f ortho2(float width,float height,float znear=0,float zfar=1)
    {
        Matrix4f result;

        cml::matrix_orthographic_RH(result,0.0f,width,height,0.0f,znear,zfar,cml::z_clip_zero);

        //CML有一个只传width,height,但结果与所期望的不一样

        return result;
    }

    inline Matrix4f ortho4(float left,float right,float bottom,float top,float znear=0,float zfar=1)
    {
        Matrix4f result;

        cml::matrix_orthographic_RH(result,left,right,bottom,top,znear,zfar,cml::z_clip_zero);

        return result;
    }

    inline Matrix4f lookAt(const cml::vector3f &eye,const cml::vector3f &target,const cml::vector3f &up)
    {
        Matrix4f result;

        cml::matrix_look_at_RH(result,eye,target,up);

        return result;
    }

    inline Matrix4f translate(const cml::vector3f &v)
    {
        Matrix4f result;

        cml::matrix_translation(result,v);

        return result;
    }

    inline Matrix4f translate(float x,float y,float z)
    {
        Matrix4f result;

        cml::matrix_translation(result,x,y,z);

        return result;
    }

    inline Matrix4f scale(const cml::vector3f &v)
    {
        Matrix4f result;

        cml::matrix_scale(result,v);

        return result;
    }

    inline Matrix4f scale(float x,float y,float z)
    {
        Matrix4f result;

        cml::matrix_scale(result,x,y,z);

        return result;
    }

    inline Matrix4f scale(float s)
    {
        Matrix4f result;

        cml::matrix_scale(result,s,s,s);

        return result;
    }

    inline Matrix4f rotate(float angle,const Vector3f &axis)
    {
        Matrix4f result;

        cml::matrix_rotation_axis_angle(result,axis,float(angle*HGL_PI/180.0f));

        return result;
    }

    inline Matrix4f rotate(float angle,float x,float y,float z)
    {
        return rotate(angle,Vector3f(x,y,z));
    }

    inline Matrix4f rotate(float angle,const cml::vector4f &axis)
    {
        return rotate(angle,axis[0],axis[1],axis[2]);
    }

    inline Vector2f normalized(const Vector2f &v)
    {
        return cml::normalize(v);
    }

    inline Vector3f normalized(const Vector3f &v)
    {
        return cml::normalize(v);
    }

    inline void normalize(Vector2f &v)
    {
        v.normalize();
    }

    inline void normalize(Vector3f &v)
    {
        v.normalize();
    }

    inline Vector3f cross(const Vector3f &v1,const Vector3f &v2)
    {
        return cml::cross(v1,v2);
    }

    inline float dot(const Vector2f &v1,const Vector2f &v2)
    {
        return cml::dot(v1,v2);
    }

    inline float dot(const Vector3f &v1,const Vector3f &v2)
    {
        return cml::dot(v1,v2);
    }
}//namespace hgl
#endif//HGL_ALGORITHM_VECTOR_MATH_CML_INCLUDE
