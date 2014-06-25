#include"../project/GameCommon/AttackValue.h"
#include<sys/time.h>
#include<iostream>

#include <hgl/TypeFunc.h>
#include <math.h>

namespace hgl
{
	namespace io
	{
		class DataInputStream;
		class DataOutputStream;
	}//namespace io

	using namespace io;
}//namespace hgl

using namespace hgl;

struct CAV
{
	union
	{
		struct
		{
			float	physics;
			float	element;
			float	light;
			float	dark;
		};

		float value[4];
	};

public:

	CAV()
	{
		Clear();
	}

	void Clear()
	{
		hgl_zero(value);
	}

	const float GetTotal()const
	{
		return physics+element+light+dark;
	}

	void operator = (const float v)
	{
		for(int i=0;i<4;i++)
			value[i]=v;
	}

	void Add(float v,int type)
	{
		value[type]+=v;
	}

	void Ceil()
	{
		physics=ceil(physics);
		element=ceil(element);
		light=ceil(light);
		dark=ceil(dark);
	}

	void Clip(int num) // 截取num位小数点
	{
		physics=hgl_clip(physics,num);
		element=hgl_clip(element,num);
		light=hgl_clip(light,num);
		dark=hgl_clip(dark,num);
	}

#define AV_BASE_COMPUT_EQU_OPERATOR(op)	void operator op(const CAV &av)	\
{	\
	for(int i=0;i<4;i++)	\
		value[i] op av.value[i];	\
}

	AV_BASE_COMPUT_EQU_OPERATOR(+=);
	AV_BASE_COMPUT_EQU_OPERATOR(-=);
	AV_BASE_COMPUT_EQU_OPERATOR(*=);
	AV_BASE_COMPUT_EQU_OPERATOR(/=);

#undef AV_BASE_COMPUT_EQU_OPERATOR

#define AV_BASE_COMPUT_OPERATOR(op)	CAV operator op(const CAV &av)const	\
									{	\
										CAV result;	\
									\
										for(int i=0;i<4;i++)	\
											result.value[i]=value[i] op av.value[i];	\
									\
										return(result);	\
									}

	AV_BASE_COMPUT_OPERATOR(+);
	AV_BASE_COMPUT_OPERATOR(-);
	AV_BASE_COMPUT_OPERATOR(*);
	AV_BASE_COMPUT_OPERATOR(/);

#undef AV_BASE_COMPUT_OPERATOR

	void operator*=(const float v)
	{
		for(int i=0;i<4;i++)
			value[i]*=v;
	}

	void operator/=(const float v)
	{
		for(int i=0;i<4;i++)
			value[i]/=v;
	}

	CAV operator * (const float v)const
	{
		CAV av=*this;

		av*=v;
		return(av);
	}

	CAV operator + (const float v)const
	{
		CAV av=*this;

		for(int i=0;i<4;i++)
			av.value[i]+=v;

		return(av);
	}

	CAV operator - (const float v)const
	{
		CAV av=*this;

		for(int i=0;i<4;i++)
			av.value[i]-=v;

		return(av);
	}

	bool operator == (const CAV &v)const
	{
		if(physics!=v.physics)return(false);
		if(light!=v.light)return(false);
		if(element!=v.element)return(false);
		if(dark!=v.dark)return(false);

		return(true);
	}

	bool operator != (const CAV &v)const
	{
		return !(operator==(v));
	}

	bool operator < (const CAV &v)const
	{
		if(physics<v.physics)return(false);
		if(light<v.light)return(false);
		if(element<v.element)return(false);
		if(dark<v.dark)return(false);

		return(true);
	}

	bool add_up_level(float n)
	{
		for(int i=0;i<4;i++)
		{
			if(value[i]>0)
				value[i]+=n;
		}

		return(true);
	}
};//struct CAV

template<typename T> void test(const char *name)
{
	timeval start_time,end_time;

	T av1;
	T av2;
	T av3;

	gettimeofday(&start_time,nullptr);

	for(int i=0;i<1000000;i++)
	{
		av1=i+1;
		av2=i+2;
		av3=i+3;

		av1=av2+av3;
		av1=av2-av3;
		av1=av2*av3;
		av1=av2/av3;

		av1+=av2;
		av1-=av2;
		av1*=av2;
		av1/=av2;
	}

	gettimeofday(&end_time,nullptr);

	long long st=start_time.tv_sec*1000*1000+start_time.tv_usec;
	long long et=end_time.tv_sec*1000*1000+end_time.tv_usec;

	std::cout<<name<< "time diff="<<et-st<<std::endl;
}

int main()
{
	test<AttackValue>("SSE ");
	test<CAV>("C ");

	return(0);
}
