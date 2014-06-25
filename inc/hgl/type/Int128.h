#ifndef HGL_INT128_INCLUDE
#define HGL_INT128_INCLUDE

#include<hgl/type/DataType.h>
namespace hgl
{
	/**
	* 128位整型数据变量,用于超大数运算
	*/
	class int128																					///int128位整型数
	{
	public:

		uint64 LowPart;
		uint64 HighPart;

	public:

		int128();
		int128(uint64 n);
		int128(uint64 HighPart,uint64 LowPart);

	    int128 operator = (const int128 &n);
		int128 operator << (int n);
		int128 operator >> (int n);

		friend int128 operator / (int128 n1,int128 n2);
		friend int128 operator * (int128 n1,int128 n2);
		friend int128 operator % (int128 n1,int128 n2);
		friend int128 operator + (int128 n1,int128 n2);
		friend int128 operator - (int128 n1,int128 n2);
		friend int128 operator += (int128 &n1,int128 n2);
		friend int128 operator -= (int128 &n1,int128 n2);
		friend int128 operator *= (int128 &n1,int128 n2);
		friend int128 operator /= (int128 &n1,int128 n2);
		friend int128 operator | (int128 n1,int128 n2);
		friend int128 operator & (int128 n1,int128 n2);

		inline int128 &operator ++()			//前置++
		{
			if (++LowPart == 0)
				++HighPart;

			return(*this);
		}

		inline int128 &operator --()			//前置--
		{
			if (LowPart-- == 0)
				HighPart--;

			return(*this);
		}

		inline int128 operator ++ (int)
		{
			int128 ret=*this;

			if (++LowPart == 0)
				++HighPart;

			return(ret);
		}

		inline int128 operator -- (int)
		{
			int128 ret=*this;

			if (LowPart-- == 0)
				HighPart--;

			return ret;
		}

		friend bool operator == (int128 n1,int128 n2);
		friend bool operator >  (int128 n1,int128 n2);
		friend bool operator <  (int128 n1,int128 n2);
		friend bool operator != (int128 n1,int128 n2);
		friend bool operator >= (int128 n1,int128 n2);
		friend bool operator <= (int128 n1,int128 n2);

		void Set(uint64 HighPart,uint64 LowPart);
		uint64 GetLowPart() {return(LowPart);}
	};//class int128

	inline uint64 int128to64(int128 n) {return(n.GetLowPart());}
	#define int64to128(high,low) (int128((high),(low)))
	#define is128plus(x) ((int)(x).HighPart>=0)

	#define INT128ERR int64to128(0x8000000000000000,0)
	#define INT128MAX int64to128(0x7fffffffffffffff,0)
}//namespace hgl

#include<hgl/Str.h>
namespace hgl
{
	template<typename T> T *u128tos(T *str,int size,int128 n)
    {
		T NumStr[128];
		int pos=0;

		do
		{
			NumStr[pos++]=int128to64(n%10)+'0';
			n=n/10;
		}while(n!=0);

		if(pos>=size)
			return(nullptr);

		for (int i=0;i<pos;i++)
			str[i]=NumStr[pos-i-1];

		str[pos]=0;

		return str;
    }

    template<> inline char *utos<char,int128>(char *str,int size,int128 n)
	{
		return u128tos(str,size,n);
	}

	template<> inline uint8 *utos<uint8,int128>(uint8 *str,int size,int128 n)
	{
		return u128tos(str,size,n);
	}

	template<> inline char16_t *utos<char16_t,int128>(char16_t *str,int size,int128 n)
	{
		return u128tos(str,size,n);
	}
}//namespace hgl
#endif//HGL_INT128_INCLUDE
