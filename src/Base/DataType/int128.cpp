#include<hgl/type/Int128.h>

namespace hgl
{
	int128::int128()
	{
	}

	int128::int128(uint64 n)
	{
		HighPart=0;
		LowPart=n;
	}

	int128::int128(uint64 HighPart,uint64 LowPart)
	{
		int128::HighPart=HighPart;
		int128::LowPart=LowPart;
	}

	int128 int128::operator = (const int128 &n)
	{
		HighPart=n.HighPart;
		LowPart=n.LowPart;

		return(*this);
	}

	int128 int128::operator << (int n)
	{
		int128 res=*this;
		while (n--)
		{
			res.HighPart<<=1;
			if (res.LowPart & 0x8000000000000000)
				res.HighPart|=1;
			res.LowPart<<=1;
		}
		return(res);
	}

	int128 int128::operator >> (int n)
	{
		int128 res=*this;
		while (n--)
		{
			res.LowPart>>=1;
			if (res.HighPart & 1)
				res.LowPart|=0x8000000000000000;
			res.HighPart>>=1;
		}
		return(res);
	}

	int128 operator / (int128 n1,int128 n2)
	{
		if (n1.HighPart==0 && n2.HighPart==0)
			return(int128(0,n1.LowPart/n2.LowPart));
		int ShiftCount=0;
		while (n1>n2)
		{
			n2=n2<<1;
			if (++ShiftCount>128)
				return(0);
		}
		int128 res=0;
		while (ShiftCount-- >= 0)
		{
			res=res<<1;
			if (n1>=n2)
			{
				n1-=n2;
				++res;
			}
			n2=n2>>1;
		}
		return(res);
	}

	int128 operator * (int128 n1,int128 n2)
	{
		if (n1<0x100000000 && n2<0x100000000)
			return(int128(0,n1.LowPart*n2.LowPart));
		int128 res=0;
		for (int I=0;I<128;I++)
		{
			if (n2.LowPart & 1)
				res+=n1;
			n1=n1<<1;
			n2=n2>>1;
		}
		return(res);
	}

	int128 operator % (int128 n1,int128 n2)
	{
		if (n1.HighPart==0 && n2.HighPart==0)
			return(int128(0,n1.LowPart%n2.LowPart));
		return(n1-n1/n2*n2);
	}

	int128 operator + (int128 n1,int128 n2)
	{
		n1.LowPart+=n2.LowPart;
		if (n1.LowPart<n2.LowPart)
			n1.HighPart++;
		n1.HighPart+=n2.HighPart;
		return(n1);
	}

	int128 operator - (int128 n1,int128 n2)
	{
		if (n1.LowPart<n2.LowPart)
			n1.HighPart--;
		n1.LowPart-=n2.LowPart;
		n1.HighPart-=n2.HighPart;
		return(n1);
	}

	int128 operator += (int128 &n1,int128 n2)
	{
		n1=n1+n2;
		return(n1);
	}

	int128 operator -= (int128 &n1,int128 n2)
	{
		n1=n1-n2;
		return(n1);
	}

	int128 operator *= (int128 &n1,int128 n2)
	{
		n1=n1*n2;
		return(n1);
	}

	int128 operator /= (int128 &n1,int128 n2)
	{
		n1=n1/n2;
		return(n1);
	}

	int128 operator | (int128 n1,int128 n2)
	{
		n1.LowPart|=n2.LowPart;
		n1.HighPart|=n2.HighPart;
		return(n1);
	}

	int128 operator & (int128 n1,int128 n2)
	{
		n1.LowPart&=n2.LowPart;
		n1.HighPart&=n2.HighPart;
		return(n1);
	}

	/*
	inline void operator -= (int128 &n1,unsigned int n2)
	{
	if (n1.LowPart<n2)
		n1.HighPart--;
	n1.LowPart-=n2;
	}


	inline void operator ++ (int128 &n)
	{
	if (++n.LowPart == 0)
		++n.HighPart;
	}


	inline void operator -- (int128 &n)
	{
	if (n.LowPart-- == 0)
		n.HighPart--;
	}
	*/

	bool operator == (int128 n1,int128 n2)
	{
		return(n1.LowPart==n2.LowPart && n1.HighPart==n2.HighPart);
	}

	bool operator > (int128 n1,int128 n2)
	{
		return((int)n1.HighPart>(int)n2.HighPart || (n1.HighPart==n2.HighPart && n1.LowPart>n2.LowPart));
	}

	bool operator < (int128 n1,int128 n2)
	{
		return((int)n1.HighPart<(int)n2.HighPart || (n1.HighPart==n2.HighPart && n1.LowPart<n2.LowPart));
	}

	bool operator != (int128 n1,int128 n2)
	{
		return(n1.LowPart!=n2.LowPart || n1.HighPart!=n2.HighPart);
	}

	bool operator >= (int128 n1,int128 n2)
	{
		return(n1>n2 || n1==n2);
	}

	bool operator <= (int128 n1,int128 n2)
	{
		return(n1<n2 || n1==n2);
	}

	void int128::Set(uint64 HighPart,uint64 LowPart)
	{
		int128::HighPart=HighPart;
		int128::LowPart=LowPart;
	}
}//namespace hgl
