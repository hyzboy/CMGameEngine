#ifndef HGL_TYPE_FUNC_INCLUDE
#define HGL_TYPE_FUNC_INCLUDE

#include<hgl/platform/Platform.h>
#include<math.h>
namespace hgl
{
	#define HGL_CONVER_TO_MEM_ALIGN(x)		((((x)+HGL_MEM_ALIGN-1)/HGL_MEM_ALIGN)*HGL_MEM_ALIGN)					//内存对齐转换宏

	#ifndef NULL
	#define NULL 0
	#endif//

    template<class T>
    inline void hgl_call_construct(T *obj)      //呼叫构造函数
    {
        new (static_cast<void *>(obj)) T();
    }

	#define HGL_BIT(n)		(1<<(n))
	#define HGL_64BIT(n)	(1L<<(n))

	template<typename T>
	inline bool hgl_is_one(const T value,int off)
	{
		return value&(1<<off);
	}

	template<typename T>
	inline bool hgl_is_zero(const T value,int off)
	{
		return !(value&(1<<off));
	}

	template<typename T>
	inline void hgl_set_one(T &value,int off)
	{
		value|=(1<<off);
	}

	template<typename T>
	inline void hgl_set_zero(T &value,int off)
	{
		value&=~T(1<<off);
	}

	template<typename T>
	inline void hgl_set_bit(T &value,int off,bool b)
	{
		if(b)
			hgl_set_one(value,off);
		else
			hgl_set_zero(value,off);
	}

	template<typename T>
	inline int hgl_bit_count(const T value)
	{
		int count=0;
		T bit=1;

		for(int i=0;i<sizeof(T);i++)
		{
			if(value&bit)
				++count;

			bit<<=1;
		}

		return count;
	}

	template<typename T>
	inline int hgl_bit_count(const T value,int size)
	{
		int count=0;
		T bit=1;

		for(int i=0;i<size;i++)
		{
			if(value&bit)
				++count;

			bit<<=1;
		}

		return count;
	}

	#define DEF_RGB_U8_TO_F(r,g,b)		{float(r)/255.0f,float(g)/255.0f,float(b)/255.0f}
	#define DEF_RGBA_U8_TO_F(r,g,b,a)	{float(r)/255.0f,float(g)/255.0f,float(b)/255.0f,float(a)/255.0f}

	#define HGL_SIZE_1KB	1024
	#define HGL_SIZE_1MB	(HGL_SIZE_1KB*1024)
	#define HGL_SIZE_1GB	(HGL_SIZE_1MB*1024)
	#define HGL_SIZE_1TB	(HGL_SIZE_1GB*1024LL)
	#define HGL_SIZE_1PB	(HGL_SIZE_1TB*1024LL)
	#define HGL_SIZE_1EB	(HGL_SIZE_1PB*1024LL)
	#define HGL_SIZE_1ZB	(HGL_SIZE_1EB*1024LL)
	#define HGL_SIZE_1YB	(HGL_SIZE_1ZB*1024LL)

	const uint8		HGL_U8_MAX		=0xFF;
	const uint16	HGL_U16_MAX		=0xFFFF;
	const uint32	HGL_U32_MAX		=0xFFFFFFFF;
	const uint64	HGL_U64_MAX		=0xFFFFFFFFFFFFFFFFUL;

	const int8		HGL_S8_MAX		=0x7F;
	const int16		HGL_S16_MAX		=0x7FFF;
	const int32		HGL_S32_MAX		=0x7FFFFFFF;
	const int64		HGL_S64_MAX		=0x7FFFFFFFFFFFFFFFLL;

	const int8		HGL_S8_MIN		=(-0x80);
	const int16		HGL_S16_MIN		=(-0x8000);

#ifdef _MSC_VER
	const int32		HGL_S32_MIN		=(-0x80000000i32);
    const int64     HGL_S64_MIN     =(-0x8000000000000000i64);
#else
    const int32     HGL_S32_MIN     =(-0x80000000L);
    const int64     HGL_S64_MIN     =(-0x8000000000000000LL);
#endif//_MSC_VER

	template<typename T> T HGL_INTEGER_MAX();
	template<typename T> T HGL_INTEGER_MIN();

	template<> inline uint8		HGL_INTEGER_MAX<uint8	>() { return HGL_U8_MAX; }
	template<> inline uint16	HGL_INTEGER_MAX<uint16	>() { return HGL_U16_MAX; }
	template<> inline uint32	HGL_INTEGER_MAX<uint32	>() { return HGL_U32_MAX; }
	template<> inline uint64	HGL_INTEGER_MAX<uint64	>() { return HGL_U64_MAX; }

	template<> inline int8		HGL_INTEGER_MAX<int8	>() { return HGL_S8_MAX; }
	template<> inline int16		HGL_INTEGER_MAX<int16	>() { return HGL_S16_MAX; }
	template<> inline int32		HGL_INTEGER_MAX<int32	>() { return HGL_S32_MAX; }
	template<> inline int64		HGL_INTEGER_MAX<int64	>() { return HGL_S64_MAX; }

	template<> inline int8		HGL_INTEGER_MIN<int8	>() { return HGL_S8_MIN; }
	template<> inline int16		HGL_INTEGER_MIN<int16	>() { return HGL_S16_MIN; }
	template<> inline int32		HGL_INTEGER_MIN<int32	>() { return HGL_S32_MIN; }
	template<> inline int64		HGL_INTEGER_MIN<int64	>() { return HGL_S64_MIN; }

	/**
	 * 星期每天枚举
	 */
	enum WeekDay
	{
		weekNone=-1,

		weekSunday,		///<星期天
		weekMonday,		///<星期一
		weekTuesday,	///<星期二
		weekWednesday,	///<星期三
		weekThursday,	///<星期四
		weekFriday,		///<星期五
		weekSaturday,	///<星期六

		weekEnd
	};

    /**
     * 月份枚举
     */
    enum Month
    {
        monthNone=0,

        monthJanuary,   ///<一月
        monthFebruary,  ///<二月
        monthMarch,     ///<三月
        monthApril,     ///<四月
        monthMay,       ///<五月
        monthJune,      ///<六月
        monthJuly,      ///<七月
        monthAugust,    ///<八月
        monthSeptember, ///<九月
        monthOctober,   ///<十月
        monthNovember,  ///<十一月
        monthDecember,  ///<十二月

        monthEnd
    };

	#define	HGL_TIME_ONE_SECOND		1
	#define	HGL_TIME_HALF_MINUTE	30
	#define	HGL_TIME_ONE_MINUTE		60
	#define	HGL_TIME_HALF_HOUR		(30*HGL_TIME_ONE_MINUTE)
	#define	HGL_TIME_ONE_HOUR		(60*HGL_TIME_ONE_MINUTE)
	#define	HGL_TIME_HALF_DAY		(12*HGL_TIME_ONE_HOUR)
	#define	HGL_TIME_ONE_DAY		(24*HGL_TIME_ONE_HOUR)
	#define HGL_TIME_ONE_WEEK		(7*HGL_TIME_ONE_DAY)
	#define HGL_TIME_ONE_YEAR		(365*HGL_TIME_ONE_DAY)

	#define HGL_HOUR_HALF_DAY		12
	#define HGL_HOUR_ONE_DAY		24

	#define HGL_DAY_ONE_WEEK		7
	#define HGL_DAY_ONE_YEAR		365

	#define HGL_MONTH_ONE_YEAR		12

	#define	HGL_MILLI_SEC_PRE_SEC	1000				//毫秒
	#define	HGL_MICRO_SEC_PER_SEC	(1000*1000)			//微秒
	#define	HGL_NANO_SEC_PER_SEC	(1000*1000*1000)	//纳秒

	const char LowerHexChar[16]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};	///<小写16进制字符
	const char UpperHexChar[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};	///<大写16进制字符

	#define HGL_E			2.7182818284590452353602874713526624977572470936999595749669676277240766303535475945713821785251664274		//欧拉数
	#define HGL_LOG2E		1.44269504088896340736
	#define HGL_LOG10E		0.434294481903251827651
	#define HGL_LN2			0.693147180559945309417
	#define HGL_LN10		2.30258509299404568402
	#define HGL_PI			3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117068
	#define HGL_PI_2		1.57079632679489661923
	#define HGL_PI_4		0.785398163397448309616
	#define HGL_SIZE_1_PI	0.318309886183790671538
	#define HGL_2_PI		0.636619772367581343076
	#define HGL_2_SQRTPI	1.12837916709551257390
	#define HGL_SQRT2		1.41421356237309504880
	#define HGL_SQRT1_2		0.707106781186547524401

	#define HGL_COS_ANG_0	1
	#define HGL_COS_ANG_45	0.707106781187
	#define HGL_COS_ANG_90	0
	#define HGL_COS_ANG_135	-0.707106781187
	#define HGL_COS_ANG_180	-1
	#define HGL_COS_ANG_225	-0.707106781187
	#define HGL_COS_ANG_270	0
	#define HGL_COS_ANG_315	0.707106781187

	#define HGL_GOLDEN_RATIO				1.6180339887498948482	//黄金比例
	#define HGL_SILVER_RATIO				2.4142135623730950488	//白银比例

    #define HGL_SPEED_OF_SOUND              331.3f          //音速(米/秒)
	#define HGL_SPEED_OF_LIGHT				299792458		//光速(米/秒)

    #define HGL_ABSOLUTE_ZERO               -273.15f        //绝对零度

	#define HGL_UNIVERSAL_GRAVITATION		(6.6742e-11)	//万有引力常数
	#define HGL_GRAVITATIONAL_ACCELERATION	9.80665			//重力加速度

#if HGL_OS == HGL_OS_Windows
	inline uint64 pow10(const int p)
	{
		uint64 value = 10;

		for (int i = 1; i < p; i++)
			value *= 10;

		return value;
	}
#endif//HGL_OS == HGL_OS_Windows

	/**
	 * 指数计算模板类
	 */
	template<int B,int N> struct Pow
	{
		enum{value=B*Pow<B,N-1>::value};
	};

	template<int B> struct Pow<B,0>
	{
		enum{value=1};				//指数为0时结果永远为1
	};

	//int quartic_of_three=Pow<3,4>::value;		//示例，求3的4次方

	/**
	 * 取适合正巧大于当前数的2次幂值
	 */
	template<typename T>
	inline T power_to_2(T value)
	{
		T result=1;

		while(result<value)
			result<<=1;

		return(result);
	}

	template<typename T> inline void hgl_swap(T &x,T &y)
	{
		T t;

		memcpy(&t,&x,sizeof(T));
		memcpy(&x,&y,sizeof(T));
		memcpy(&y,&t,sizeof(T));
	}

	template<typename T> inline T hgl_abs(const T &v){return (v>=0?v:-v);}

	template<typename T> inline T hgl_min(const T &a,const T &b){return (a>b?b:a);}
	template<typename T> inline T hgl_max(const T &a,const T &b){return (a>b?a:b);}

	template<typename T> inline T hgl_min(const T *data,int count,T min_value)
	{
		T value=min_value;

		for(int i=0;i<count;i++)
		{
			if(*data<value)
				value=*data;

			++data;
		}

		return(value);
	}

	template<typename T> inline T hgl_max(const T *data,int count,T max_value)
	{
		T value=max_value;

		for(int i=0;i<count;i++)
		{
			if(*data>value)
				value=*data;

			++data;
		}

		return(value);
	}

	/**
	 * 角度转弧度
	 */
	inline double hgl_ang2rad(const double ang)
	{
		return ang*(HGL_PI/180.0f);
	}

	/**
	 * 弧度转角度
	 */
	inline double hgl_rad2ang(const double rad)
	{
		return rad*(180.0f/HGL_PI);
	}

	/**
     * 浮点数截取小数点后指定位度
     * @param value 要截取的浮点数
     * @param num 要截取的位数
     */
	template<typename T>
	inline T hgl_clip_float(const T value,const int num)
	{
		if(num<=0)
			return int64(value);

		if(value==0)return(0);

		double per=pow(10,num);

		return double(floor(value*per))/per;
	}

	/**
     * 等值类型复制
     */
	template<typename T>
	inline void hgl_equcpy(T &dst,const T &src)
	{
		memcpy(&dst,&src,sizeof(T));
	}

	/**
     * 不同类型数据块复制
     */
	template<typename S,typename D>
	inline void hgl_cpy(D *dst,const S *src,const size_t count)
	{
		for(size_t i=0;i<count;i++)
		{
			*dst=D(*src);
			++dst;
			++src;
		}
	}

	/**
     * 同类型数据块复制
     */
	template<typename T>
	inline void hgl_typecpy(T *dst,const T *src,const size_t count)
	{
		memcpy(dst,src,count*sizeof(T));
	}

	/**
     * 同类型数据块移动
     */
	template<typename T>
	inline void hgl_typemove(T *dst,const T *src,const size_t count)
	{
		memmove(dst,src,count*sizeof(T));
	}

	/**
     * 指定类型数据块赋值
     */
	template<typename T>
	inline void hgl_set(T *data,const T value,const size_t count)
	{
		for(size_t i=0;i<count;i++)
		{
			*data=value;
			++data;
		}
	}

	/**
     * 指定类型数据块清0
     */
	template<typename T>
	inline void hgl_zero(T *data,const size_t count)
	{
		memset(data,0,count*sizeof(T));
	}

	/**
     * 指定类型数据清0
     */
	template<typename T>
	inline void hgl_zero(T &data)
	{
		memset(&data,0,sizeof(T));
	}
}//namespace hgl

/**
* 比较处理模板基类
*/
template<typename T> class Comparator																///比较处理模板基类
{
public:

	/**
	* 比较函数，需被特例化或派生实现
	*/
	virtual int compare(const T &a,const T &b)const
	{
		return 0;	//	如 return(a-b); ，但这个函数正确情况下不应该会被调用
	}

	/**
	 * 交换函数
	 */
	virtual void exchange(T &a,T &b)
	{
		T t;

		memcpy(&t,&a,sizeof(T));
		memcpy(&a,&b,sizeof(T));
		memcpy(&b,&t,sizeof(T));
	}

	/**
	 * 复制数据
	 */
	virtual void cpy(T *t,T *s)
	{
		memcpy(t,s,sizeof(T));
	}
};//class Comparator

//针对原生类型的特例化版本，做适当加速
#define COMPARATOR_ORIGIN_TYPE(type)	template<> class Comparator<type>	\
										{	\
										public:	\
											int compare(const type &a,const type &b)const{return a-b;}	\
											void exchange(type &a,type &b){type t;t=a;a=b;b=t;}	\
											void cpy(type *t,type *s){*t=*s;}	\
										};

	COMPARATOR_ORIGIN_TYPE(hgl::int8)
	COMPARATOR_ORIGIN_TYPE(hgl::int16)
	COMPARATOR_ORIGIN_TYPE(hgl::int32)
	COMPARATOR_ORIGIN_TYPE(hgl::int64)

	COMPARATOR_ORIGIN_TYPE(hgl::uint8)
	COMPARATOR_ORIGIN_TYPE(hgl::uint16)
	COMPARATOR_ORIGIN_TYPE(hgl::uint32)
	COMPARATOR_ORIGIN_TYPE(hgl::uint64)

	COMPARATOR_ORIGIN_TYPE(float)
	COMPARATOR_ORIGIN_TYPE(double)

	COMPARATOR_ORIGIN_TYPE(char)
	COMPARATOR_ORIGIN_TYPE(wchar_t)

	#ifdef HGL_CPP11
	COMPARATOR_ORIGIN_TYPE(u16char)
	COMPARATOR_ORIGIN_TYPE(char32_t)
	#endif//HGL_CPP11
#undef COMPARATOR_ORIGIN_TYPE
#endif//HGL_TYPE_FUNC_INCLUDE
