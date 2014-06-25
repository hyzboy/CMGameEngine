#ifndef HGL_THREAD_ATOMIC_APR_INCLUDE
#define HGL_THREAD_ATOMIC_APR_INCLUDE

#include<apr_atomic.h>
namespace hgl
{
	typedef int				aint;

	typedef volatile aint	avint;
	typedef const avint		cavint;

	/**
	* 原子整型数据，基于原子操作的int32型变量模拟封装</p>
	* 注：老旧的Linux/32bit下原子仅支持24位，但我们设定为不支持旧的Linux
	*/
	class atom_int																				///原子int32数据
	{
		avint value;

	public:

		atom_int(){value=0;}
		atom_int(cavint new_value){operator=(new_value);}

		inline aint operator  *= 		(cavint v)		{return operator=(value *=v);}
		inline aint operator  /= 		(cavint v)		{return operator=(value /=v);}
		inline aint operator  %= 		(cavint v)		{return operator=(value %=v);}
		inline aint operator >>= 		(cavint v)		{return operator=(value>>=v);}
		inline aint operator <<= 		(cavint v)		{return operator=(value<<=v);}

		inline aint operator ! 			()const			{return !value;}
		inline aint operator ~ 			()const			{return ~value;}

		inline		operator T			()				{return value;}
		inline 		operator const aint	()const			{return value;}
		inline bool operator !			()				{return !value;}

		inline bool operator == 		(cavint v)const	{return value==v;}
		inline bool operator != 		(cavint v)const	{return value!=v;}

		inline aint operator &=	(cavint v)			{return operator=(value &=v);}
		inline aint operator |=	(cavint v)			{return operator=(value |=v);}
		inline aint operator ^=	(cavint v)			{return operator=(value ^=v);}

//		inline		operator aint	()				{return apr_atomic_read32(&value);}

		inline aint operator =	(cavint new_value)	{		apr_atomic_set32(&value,new_value);	return value;	}

		inline aint operator +=	(cavint add_value)	{		apr_atomic_add32(&value,add_value);	return value;	}
		inline aint operator -=	(cavint sub_value)	{		apr_atomic_sub32(&value,sub_value);	return value;	}

		inline aint operator ++	()					{		apr_atomic_inc32(&value);			return value;	}
		inline aint operator ++	(int)				{return apr_atomic_inc32(&value);							}
		inline aint operator --	()					{		apr_atomic_dec32(&value);			return value;	}
		inline aint operator --	(int)				{return apr_atomic_dec32(&value);							}	//这个在linux下似乎结果不对，看未来版本升级
	};//class atom_int
#endif//HGL_THREAD_ATOMIC_APR_INCLUDE
