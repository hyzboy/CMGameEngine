#ifndef HGL_TYPE_ARRAY_STRING_INCLUDE
#define HGL_TYPE_ARRAY_STRING_INCLUDE

#include<hgl/type/BaseString.h>
#include<hgl/CompOperator.h>
namespace hgl
{
	/**
	 * 数组字符串类
	 */
	template<typename T,int MAX_SIZE> class ArrayString
	{
		T data[MAX_SIZE+1];
		int str_len;

		typedef ArrayString<T,MAX_SIZE> SelfClass;

	public:

		ArrayString()
		{
			data[0]=0;
			str_len=0;
		}

		ArrayString(const T *str,int len=-1){Set(str,len);}
		ArrayString(const BaseString<T> &str){Set(str.c_str(),str.Length());}

		void Set(const T *str,int len=-1)
		{
			if(!str||len==0)
			{
				data[0]=0;
				str_len=0;
				return;
			}

			if(len==-1)
				len=hgl::strlen(str);

			if(len>=MAX_SIZE)
				str_len=MAX_SIZE;
			else
				str_len=len;

			memcpy(data,str,str_len*sizeof(T));
			data[str_len]=0;
		}

		const int Length()const{return str_len;}
		const bool IsEmpty()const{return str_len<=0;}

		const T *c_str()const{return data;}

		void operator=(const T *			str){Set(str);}
		void operator=(const SelfClass &	str){Set(str.c_str(),str.Length());}
		void operator=(const BaseString<T> &str){Set(str.c_str(),str.Length());}

		int Comp(const T *				str	)const{return hgl::strcmp(data,str);}
		int Comp(const SelfClass &		str	)const{return hgl::strcmp(data,str.data);}
		int Comp(const BaseString<T> &	str	)const{return hgl::strcmp(data,str.c_str());}

		CompOperator(const T *				,Comp);
		CompOperator(const SelfClass &		,Comp);
		CompOperator(const BaseString<T> &	,Comp);
	};//template<typename T,int MAX_SIZE> class ArrayString
}//namespace hgl
#endif//HGL_TYPE_ARRAY_STRING_INCLUDE
