#ifndef HGL_THREAD_ATOMIC_GNU_INCLUDE
#define HGL_THREAD_ATOMIC_GNU_INCLUDE

namespace hgl
{
	template<typename T> class atom
	{
		volatile T value;

	public:

		atom(){value=0;}
		atom(const volatile T new_value){operator=(new_value);}

		inline T	operator ->(){return value;}

		inline T	operator  *= 		(const volatile T v)		{return operator=(value *=v);}
		inline T	operator  /= 		(const volatile T v)		{return operator=(value /=v);}
		inline T	operator  %= 		(const volatile T v)		{return operator=(value %=v);}
		inline T	operator >>= 		(const volatile T v)		{return operator=(value>>=v);}
		inline T	operator <<= 		(const volatile T v)		{return operator=(value<<=v);}

		inline T	operator ! 			()const						{return !value;}
		inline T	operator ~ 			()const						{return ~value;}

		inline		operator T			()							{return value;}
		inline 		operator const T	()const						{return value;}
		inline bool operator !			()							{return !value;}

		inline bool operator == 		(const volatile T v)const	{return value==v;}
		inline bool operator != 		(const volatile T v)const	{return value!=v;}

		inline T	operator =	(const volatile T new_value)	{__sync_lock_test_and_set(&value,new_value);	return value;}

		inline T	operator +=	(const volatile T add_value)	{return __sync_add_and_fetch(&value,add_value);}
		inline T	operator -=	(const volatile T sub_value)	{return __sync_sub_and_fetch(&value,sub_value);}

		inline T	operator &=	(const volatile T v)			{return __sync_and_and_fetch(&value,v);}
		inline T	operator |=	(const volatile T v)			{return __sync_or_and_fetch (&value,v);}
		inline T	operator ^=	(const volatile T v)			{return __sync_xor_and_fetch(&value,v);}

		inline T	operator ++	()								{return __sync_add_and_fetch(&value,1);}	//前置++
		inline T	operator ++	(int)							{return __sync_fetch_and_add(&value,1);}	//后置++
		inline T	operator --	()								{return __sync_sub_and_fetch(&value,1);}
		inline T	operator --	(int)							{return __sync_fetch_and_sub(&value,1);}
	};//template<typename T> class atom
}//namespace hgl
#endif//HGL_THREAD_ATOMIC_GNU_INCLUDE
