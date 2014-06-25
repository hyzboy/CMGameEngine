#ifndef HGL_THREAD_ATOMIC_OSX_INCLUDE
#define HGL_THREAD_ATOMIC_OSX_INCLUDE

#include<libkern/OSAtomic.h>

namespace hgl
{
	typedef int				aint;

	typedef volatile aint	avint;
	typedef const avint		cavint;

	template<typename T> class atom																	///原子数据
	{
		avint value;

	public:

		atom(){value=0;}
		atom(cavint new_value){operator=(new_value);}

		inline aint operator  *= 		(cavint v)			{return operator=(value *=v);}
		inline aint operator  /= 		(cavint v)			{return operator=(value /=v);}
		inline aint operator  %= 		(cavint v)			{return operator=(value %=v);}
		inline aint operator >>= 		(cavint v)			{return operator=(value>>=v);}
		inline aint operator <<= 		(cavint v)			{return operator=(value<<=v);}

		inline aint operator ! 			()const				{return !value;}
		inline aint operator ~ 			()const				{return ~value;}

		inline 		operator const aint	()const				{return value;}
		inline bool operator !			()					{return !value;}

		inline bool operator == 		(cavint v)const		{return value==v;}
		inline bool operator != 		(cavint v)const		{return value!=v;}
		inline aint operator =			(cavint new_value)	{value=new_value;return value;}
		inline aint operator ++			()					{return OSAtomicIncrement32(&value);}
		inline aint operator --			()					{return OSAtomicDecrement32(&value);}
		inline aint operator +=			(cavint add_value)	{return;}
		inline aint operator -=			(cavint sub_value)	{return;}

		inline aint operator &=			(cavint v)			{return operator=(value &=v);}
		inline aint operator |=			(cavint v)			{return operator=(value |=v);}
		inline aint operator ^=			(cavint v)			{return operator=(value ^=v);}

		inline aint operator ++			(int)				{aint ret=value;operator++();return ret;}//后置++
		inline aint	operator --			(int)   			{volatile T ret=value;operator--();return ret;}//后置--
	};//class atom
}//namespace hgl
#endif//HGL_THREAD_ATOMIC_OSX_INCLUDE
