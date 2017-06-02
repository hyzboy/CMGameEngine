#ifndef HGL_COLOR_4_FLOAT_INCLUDE
#define HGL_COLOR_4_FLOAT_INCLUDE

#include<hgl/type/Color3f.h>
namespace hgl
{
	#define HGL_FLOAT_TO_U32(c1,c2,c3,c4)	uint32(	\
													(uint8(c1 * 255) << 24	)	|	\
													(uint8(c2 * 255) << 16	)	|	\
													(uint8(c3 * 255) << 8	)	|	\
													(uint8(c4 * 255)		)	)

	#define HGL_FLOAT_TO_RGBA8(r,g,b,a)	HGL_FLOAT_TO_U32(r,g,b,a)
	#define HGL_FLOAT_TO_BGRA8(r,g,b,a)	HGL_FLOAT_TO_U32(b,g,r,a)
	#define HGL_FLOAT_TO_ARGB8(r,g,b,a)	HGL_FLOAT_TO_U32(a,r,g,b)
	#define HGL_FLOAT_TO_ABGR8(r,g,b,a)	HGL_FLOAT_TO_U32(a,b,g,r)

	/**
	* r,g,b,a四原色类
	*/
	class Color4f                                                                                   ///RGBA四原色类
	{
		void Clamp();

	public:

		float r,g,b,a;																				///<rgba 4原色

	public:

		Color4f(){r=0,g=0,b=0,a=1;}												  			        ///<本类构造函数
		Color4f(COLOR_ENUM ce){Use(ce,1);}															///<本类构造函数
		Color4f(COLOR_ENUM ce,float ta){Use(ce,ta);}												///<本类构造函数
		Color4f(float v){r=v,g=v,b=v,a=1;Clamp();}      										    ///<本类构造函数
		Color4f(float vr,float vg,float vb,float va){r=vr,g=vg,b=vb;a=va;Clamp();}				    ///<本类构造函数
		Color4f(const Color3f &v,float va=1){Set(v,va);} 											///<本类构造函数

		void Use(COLOR_ENUM,float);
		void Zero(){r=0,g=0,b=0,a=0;}                                                               ///<全清为0
		void One() {r=1,g=1,b=1,a=1;}                                                               ///<全清为1

		void MergeAlpha(const Color4f &v,float va){r=v.r;g=v.g;b=v.b;a=v.a*va;Clamp();}				///<混合Alpha

		void Set(const float *rgba){r=rgba[0];g=rgba[1];b=rgba[2];a=rgba[3];Clamp();}
		void Set(const Color3f &v,float va=1){r=v.r;g=v.g;b=v.b;a=va;Clamp();}						///<设置颜色
		void Set(float vr,float vg,float vb){r=vr,g=vg,b=vb;Clamp();}			                    ///<设置颜色
		void Set(float vr,float vg,float vb,float va){r=vr,g=vg,b=vb;a=va;Clamp();}			        ///<设置颜色
		void Set4i(int vr,int vg,int vb,int va)                                                     ///<按0-255的范围值设置颜色
			{r=float(vr)/255.0f,g=float(vg)/255.0f,b=float(vb)/255.0f;a=float(va)/255.0f;Clamp();}

		void SetLum(float v){if(v<0)Zero();else if(v>1)One();else{r=v;g=v;b=v;}}                    ///<设置颜色

		void To(float,float,float,float);                                                           ///<转换到另一颜色
		void To(const Color4f &c,float v){To(c.r,c.g,c.b,v);}                                       ///<转换到另一颜色
		void To(const Color3f &c,float v){To(c.r,c.g,c.b,v);}                                       ///<转换到另一颜色

		uint32 ToRGBA8()const{ return HGL_FLOAT_TO_RGBA8(r, g, b, a); }								///<输出一个rgba8格式的颜色数据
		uint32 ToBGRA8()const{ return HGL_FLOAT_TO_BGRA8(r, g, b, a); }								///<输出一个bgra8格式的颜色数据
		uint32 ToARGB8()const{ return HGL_FLOAT_TO_ARGB8(r, g, b, a); }								///<输出一个argb8格式的颜色数据
		uint32 ToABGR8()const{ return HGL_FLOAT_TO_ABGR8(r, g, b, a); }								///<输出一个abgr8格式的颜色数据

		void Grey();																			    ///<将当前色彩变成灰色

		//操作符重载
		void operator = (const float *v){r=*v++;g=*v++;b=*v++;a=*v;}
		void operator = (const Color3f &v){r=v.r;g=v.g;b=v.b;a=1;}
		void operator = (const Color4f &v){r=v.r;g=v.g;b=v.b;a=v.a;}
	 	void operator = (COLOR_ENUM ce){Use(ce,a);}

		bool operator == (const Color4f &);
		bool operator != (const Color4f &);

		void operator += (const Color4f &v){r+=v.r;g+=v.g;b+=v.b;a+=v.a;Clamp();}
		void operator -= (const Color4f &v){r-=v.r;g-=v.g;b-=v.b;a-=v.a;Clamp();}
		void operator *= (const Color4f &v){r*=v.r;g*=v.g;b*=v.b;a*=v.a;Clamp();}
		void operator /= (const Color4f &v){r/=v.r;g/=v.g;b/=v.b;a/=v.a;Clamp();}

		void operator += (const Color3f &v){r+=v.r;g+=v.g;b+=v.b;Clamp();}
		void operator -= (const Color3f &v){r-=v.r;g-=v.g;b-=v.b;Clamp();}
		void operator *= (const Color3f &v){r*=v.r;g*=v.g;b*=v.b;Clamp();}
		void operator /= (const Color3f &v){r/=v.r;g/=v.g;b/=v.b;Clamp();}

		void operator *= (float v){r*=v;g*=v;b*=v;a*=v;Clamp();}
		void operator /= (float v){r/=v;g/=v;b/=v;a/=v;Clamp();}

		Color4f operator + (const Color4f &v){return(Color4f(r+v.r,g+v.g,b+v.b,a+v.a));}
		Color4f operator - (const Color4f &v){return(Color4f(r-v.r,g-v.g,b-v.b,a-v.a));}
		Color4f operator * (const Color4f &v){return(Color4f(r*v.r,g*v.g,b*v.b,a*v.a));}
		Color4f operator / (const Color4f &v){return(Color4f(r/v.r,g/v.g,b/v.b,a/v.a));}

		Color4f operator * (float v){return(Color4f(r*v,g*v,b*v,a*v));}
		Color4f operator / (float v){return(Color4f(r/v,g/v,b/v,a/v));}

		operator float *() const {return((float *)this);}										    //使得本类可以直接当做float *使用
		operator const float *() const {return((const float *)this);}							    //使得本类可以直接当做const float *使用
	};//class Color4f

	#define DEF_RGBA_U8_TO_COLOR4F(r,g,b,a)		Color4f(float(r)/255.0f,float(g)/255.0f,float(b)/255.0f,float(a)/255.0f)
}//namespace hgl
#endif//HGL_COLOR_4_FLOAT_INCLUDE
