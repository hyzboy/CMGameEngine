#ifndef HGL_ALGORITHM_VECTOR_MATH_GLM_INCLUDE
#define HGL_ALGORITHM_VECTOR_MATH_GLM_INCLUDE

#ifdef _MSC_VER
#pragma warning(disable:4244)           // double -> int 精度丢失警告
#endif//_MSC_VER

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ================================================================================
// OpenGL Mathematics (GLM)
// --------------------------------------------------------------------------------
// GLM can be distributed and/or modified under the terms of either
// a) The Happy Bunny License, or b) the MIT License.
//
// ================================================================================
// The Happy Bunny License (Modified MIT License)
// --------------------------------------------------------------------------------
// Copyright (c) 2005 - 2015 G-Truc Creation
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// Restrictions:
//  By making use of the Software for military purposes, you choose to make a
//  Bunny unhappy.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// ================================================================================
// The MIT License
// --------------------------------------------------------------------------------
// Copyright (c) 2005 - 2015 G-Truc Creation
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

namespace hgl
{
    typedef glm::vec2 Vector2f;
    typedef glm::vec3 Vector3f;
    typedef glm::vec4 Vector4f;

    typedef glm::mat3 Matrix3f;
    typedef glm::mat4 Matrix4f;

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
        return Matrix4f();
    }

    inline Matrix4f inverse(const Matrix4f &m)
    {
        return glm::inverse(m);
    }

    inline Matrix4f perspective_yfov(float fovy,float aspect,float znear,float zfar)
    {
        return glm::perspectiveRH(fovy,aspect,znear,zfar);
    }

//     inline Matrix4f perspective_wh(float width,float height,float znear,float zfar)
//     {
//         return Matrix4f::OpenGLPerspProjRH(znear,zfar,width,height);
//     }

    inline Matrix4f ortho2d(float width,float height,float znear=0,float zfar=1)
    {
        return glm::ortho(0.0f,width,height,0.0f,znear,zfar);
    }

    //inline Matrix4f ortho4(float left,float right,float bottom,float top,float znear=0,float zfar=1);

    inline Matrix4f lookAt(const Vector3f &eye,const Vector3f &target,const Vector3f &up)
    {
        return glm::lookAt(eye,target,up);
    }

    inline Matrix4f translate(const Vector3f &v)
    {
        return glm::translate(Matrix4f(1.0f),v);
    }

    inline Matrix4f translate(float x,float y,float z)
    {
        return glm::translate(Matrix4f(1.0f),Vector3f(x,y,z));
    }

    inline Matrix4f scale(const Vector3f &v)
    {
        return glm::scale(Matrix4f(1.0f),v);
    }

    inline Matrix4f scale(float x,float y,float z)
    {
        return glm::scale(Matrix4f(1.0f),Vector3f(x,y,z));
    }

    inline Matrix4f scale(float s)
    {
        return glm::scale(Matrix4f(1.0f),Vector3f(s,s,s));
    }

    inline Matrix4f rotate(float angle,const Vector3f &axis)
    {
        return glm::rotate(Matrix4f(1.0f),angle,axis);
    }

    inline Matrix4f rotate(float angle,float x,float y,float z)
    {
        return glm::rotate(Matrix4f(1.0f),angle,Vector3f(x,y,z));
    }

    inline Matrix4f rotate(float angle,const Vector4f &axis)
    {
        return glm::rotate(Matrix4f(1.0f),angle,Vector3f(axis.x,axis.y,axis.z));
    }

	inline Vector3f rotate(const Vector3f &v3f, float angle, const Vector3f &axis)
	{
		Vector4f result = rotate(angle, axis)*Vector4f(v3f, 1.0f);

		return Vector3f(result.x,result.y,result.z);
	}

    template<typename T>
    inline T normalized(const T &v)
    {
        return glm::normalize(v);
    }

    template<typename T>
    inline void normalize(T &v)
    {
        v=glm::normalize(v);
    }

    template<typename T1,typename T2>
    inline Vector3f cross(const T1 &v1,const T2 &v2)
    {
        return glm::cross(v1,v2);
    }

    template<typename T>
    inline float dot(const T &v1,const T &v2)
    {
        return glm::dot(v1,v2);
    }
}//namespace hgl
#endif//HGL_ALGORITHM_VECTOR_MATH_GLM_INCLUDE
