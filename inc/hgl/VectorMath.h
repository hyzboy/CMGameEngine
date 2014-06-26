#ifndef HGL_VECTOR_MATH_INCLUDE
#define HGL_VECTOR_MATH_INCLUDE

#ifdef _MSC_VER
#pragma warning(disable:4244)			// double -> int 精度丢失警告
#endif//_MSC_VER

#include<hgl/type/DataType.h>
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
 * 		This library is licensed under the Apache 2 license. I am not a lawyer, but to me that
 * license means that you can use this code for any purpose, both commercial and closed source.
 * You are however restricted from claiming you wrote it yourself, and cannot hold me liable
 * for anything over this code.
 *		I acknowledge that most of the non-trivial math routines are taken off a book or a
 * research paper. In all places, I have tried to be diligent to properly attribute the original
 * source. Please contact me if you feel I have misattributed something.
 */

namespace hgl
{
	double Lsin(int angle);										///<低精度sin计算,注意传入的参数为角度而非弧度
	double Lcos(int angle);										///<低精度cos计算,注意传入的参数为角度而非弧度
	void Lsincos(int angle,double &s,double &c);				///<低精度sin+cos计算,注意传入的参数为角度而非弧度

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

	inline void vec2to3(Vector3f &dst,const Vector2f &src,const float &z)
	{
		dst.x=src.x;
		dst.y=src.y;
		dst.z=z;
	}

	inline Vector3f vec2to3(const Vector2f &src,const float &z)
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

	inline Matrix4f ortho2(float width,float height,float znear=0,float zfar=1)
	{
		return Matrix4f::OpenGLOrthoProjRH(znear,zfar,width,height);
	}

	//inline Matrix4f ortho4(float left,float right,float bottom,float top,float znear=0,float zfar=1);

	inline Matrix4f lookAt(const Vector3f &eye,const Vector3f &target,const Vector3f &up)
	{
		return Matrix4f::LookAt(eye,target,up,up);
	}

	inline Matrix4f translate(const Vector3f &v)
	{
		Matrix4f result;

		result.SetTranslatePart(v);

		return result;
	}

	inline Matrix4f translate(float x,float y,float z)
	{
		Matrix4f result;

		result.SetTranslatePart(x,y,z);

		return result;
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
		Matrix4f result;

		result.SetRotatePart(axis,angle);

		return result;
	}

	inline Matrix4f rotate(float angle,float x,float y,float z)
	{
		return rotate(angle,Vector3f(x,y,z));
	}

	inline Matrix4f rotate(float angle,const Vector4f &axis)
	{
		return rotate(angle,axis.x,axis.y,axis.z);
	}

	template<typename T>
	inline T normalized(const T &v)
	{
		return v.Normalized();
	}

	template<typename T>
	inline void normalize(T &v)
	{
		v=v.Normalized();
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

	inline float length_squared(const Vector2f &v)
	{
		return (v.x*v.x)+(v.y*v.y);
	}

	inline float length_squared_2d(const Vector3f &v)
	{
		return (v.x*v.x)+(v.y*v.y);
	}

	inline float length_squared(const Vector3f &v)
	{
		return (v.x*v.x)+(v.y*v.y)+(v.z*v.z);
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

	template<typename T1,typename T2>
	inline float length_squared(const T1 &v1,const T2 &v2)
	{
		const float x=(v1.x-v2.x);
		const float y=(v1.y-v2.y);

		return x*x+y*y;
	}

	template<typename T1,typename T2>
	inline float length(const T1 &v1,const T2 &v2)
	{
		return sqrt(length_squared(v1,v2));
	}

	inline float length_squared(const Vector3f &v1,const Vector3f &v2)
	{
		const float x=(v1.x-v2.x);
		const float y=(v1.y-v2.y);
		const float z=(v1.z-v2.z);

		return x*x+y*y+z*z;
	}

	template<typename T1,typename T2>
	inline float length_squared_2d(const T1 &v1,const T2 &v2)
	{
		const float x=(v1.x-v2.x);
		const float y=(v1.y-v2.y);

		return x*x+y*y;
	}

	inline float length(const Vector3f &v1,const Vector3f &v2)
	{
		return sqrt(length_squared(v1,v2));
	}

	template<typename T1,typename T2>
	inline float length_2d(const T1 &v1,const T2 &v2)
	{
		return sqrt(length_squared_2d(v1,v2));
	}

	inline Vector2f	to(const Vector2f &start,const Vector2f &end,float pos)
	{
		return Vector2f(start.x+(end.x-start.x)*pos,
						start.y+(end.y-start.y)*pos);
	}

	inline Vector3f	to(const Vector3f &start,const Vector3f &end,float pos)
	{
		return Vector3f(start.x+(end.x-start.x)*pos,
						start.y+(end.y-start.y)*pos,
						start.z+(end.z-start.z)*pos);
	}

	template<typename T>
	inline void to_2d(T &result,const T &start,const T &end,float pos)
	{
		result.x=start.x+(end.x-start.x)*pos;
		result.y=start.y+(end.y-start.y)*pos;
	}

	inline float ray_angle_cos(const Ray &ray,const vec &pos)
	{
		return ray.dir.Dot((pos-ray.pos).Normalized());
	}

	/**
	 * 做一个2D旋转计算
	 * @param result 结果
	 * @param source 原始点坐标
	 * @param center 圆心坐标
	 * @param ang 旋转角度
	 */
	template<typename T1,typename T2,typename T3>
	inline void rotate2d(T1 &result,const T2 &source,const T3 &center,const double &ang)
	{
		double as,ac;
		double nx,ny;

// 		as=sin(ang*(HGL_PI/180.0f));
// 		ac=cos(ang*(HGL_PI/180.0f));
		//sincos(ang*(HGL_PI/180.0f),&as,&ac);		//在80x87指令上，sin/cos是一个指令同时得出sin和cos，所以可以这样做
		Lsincos(ang,as,ac);							//低精度sin/cos计算

		result.x=center.x+((source.x-center.x)*ac-(source.y-center.y)*as);
		result.y=center.y+((source.x-center.x)*as+(source.y-center.y)*ac);
	}
}//namespace hgl
#endif//HGL_VECTOR_MATH_INCLUDE
