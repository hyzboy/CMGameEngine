#ifndef HGL_CRYPT_VALUE_INCLUDE
#define HGL_CRYPT_VALUE_INCLUDE

#include<stdlib.h>
namespace hgl
{
	/**
	* 加密型变量类
	*/
	template<typename T,typename RT> class CryptValue                                          ///加密型变量类
	{
	protected:

		T value;
		T key;

	protected:

		virtual T GetValue()
		{
			T result=value^key;

			SetValue(result);

			return(result);
		}

		virtual void SetValue(T v)
		{
			RT(&key);

			value=v^key;
		}

	public:

		CryptValue()
		{
			value=0;
			key=0;
		}

		CryptValue(T v)
		{
			SetValue(v);
		}

		operator T () {return(GetValue());}

		void operator = (T v){SetValue(v);}

		T operator + (T v){return(GetValue()+v);}
		T operator - (T v){return(GetValue()-v);}
		T operator * (T v){return(GetValue()*v);}
		T operator / (T v){return(GetValue()/v);}
		T operator % (T v){return(GetValue()%v);}
        T operator & (T v){return(GetValue()&v);}
        T operator | (T v){return(GetValue()|v);}

		void operator += (T v){SetValue(GetValue()+v);}
		void operator -= (T v){SetValue(GetValue()-v);}
		void operator *= (T v){SetValue(GetValue()*v);}
		void operator /= (T v){SetValue(GetValue()/v);}
        void operator %= (T v){SetValue(GetValue()%v);}
        void operator &= (T v){SetValue(GetValue()&v);}
        void operator |= (T v){SetValue(GetValue()|v);}

		T operator ++ (int) {T r=GetValue();SetValue(r+1);return(r);}                           		///<后置++
		T operator -- (int) {T r=GetValue();SetValue(r-1);return(r);}                           		///<后置--

		T operator ++ () {SetValue(GetValue()+1);return(GetValue());}                             	///<前置++
		T operator -- () {SetValue(GetValue()-1);return(GetValue());}                             	///<前置--

		bool operator > (const T &v){return(GetValue()> v);}
		bool operator >=(const T &v){return(GetValue()>=v);}
		bool operator < (const T &v){return(GetValue()< v);}
		bool operator <=(const T &v){return(GetValue()<=v);}

		bool operator ==(const T &v){return(GetValue()==v);}
		bool operator !=(const T &v){return(GetValue()!=v);}
	};//class CryptValue

	typedef CryptValue<int,Random32>	crypt_int;
	typedef CryptValue<uint,rand_r>		crypt_uint;

	typedef CryptValue<int32,Random32>	crypt_int32;
	typedef CryptValue<uint32,rand_r>	crypt_uint32;

	typedef CryptValue<int64,lrand48>	crypt_int64;
	typedef CryptValue<uint64,lrand48>	crypt_uint64;

	typedef CryptValue<float,drand48>	crypt_float;
	typedef CryptValue<double,drand48>	crypt_double;
}//namespace hgl
#endif//HGL_CRYPT_VALUE_INCLUDE
