#include<iostream>

using namespace std;

#define HGL_DEF_EVENT_PARAM_TYPE(type,name)	type name;operator type(){return name;}

union EventParam			//��������
{
	HGL_DEF_EVENT_PARAM_TYPE(void *,		void_pointer	);
	HGL_DEF_EVENT_PARAM_TYPE(char,			c				);
	HGL_DEF_EVENT_PARAM_TYPE(char *,		str				);
	HGL_DEF_EVENT_PARAM_TYPE(char16_t,		wc				);
	HGL_DEF_EVENT_PARAM_TYPE(char16_t *,		wide_str		);
	HGL_DEF_EVENT_PARAM_TYPE(int,			i				);
	HGL_DEF_EVENT_PARAM_TYPE(int *,			int_pointer		);
	HGL_DEF_EVENT_PARAM_TYPE(unsigned int,	u				);
	HGL_DEF_EVENT_PARAM_TYPE(unsigned int *,uint_pointer	);
	HGL_DEF_EVENT_PARAM_TYPE(float,			f				);
	HGL_DEF_EVENT_PARAM_TYPE(float *,		float_pointer	);
	HGL_DEF_EVENT_PARAM_TYPE(double,		d				);
	HGL_DEF_EVENT_PARAM_TYPE(double *,		double_pointer	);
};//union EventParam

#undef HGL_DEF_EVENT_PARAM_TYPE

struct EventBase
{
protected:

	int param_count;
	EventParam *ep;
	EventParam result;

public:

	EventBase(int pc)
	{
		param_count=pc;

		if(pc>0)
			ep=new EventParam[param_count];
		else
		{
			param_count=0;
			ep=0;
		}
	}

	virtual ~EventBase()
	{
		delete[] ep;
	}

	template<typename T>
	bool set_param(int index,T &p)
	{
		if(index<0||index>=param_count)return(false);

		ep[index]=p;
		return(true);
	}

	const int get_param_count()const{return param_count;}

	virtual void call(){}

	template<typename T>
	void get_result(T &t)
	{
		t=result;
	}
};

template<typename RT>
struct EventFunc0:public EventBase
{
	typedef RT (*ev_func)();

	ev_func func;
	RT result;

	EventFunc0(ev_func f):EventBase(0)
	{
		func=f;
	}

	void call()
	{
		result=func();
	}
};

template<>
struct EventFunc0<void>:public EventBase
{
	typedef void (*ev_func)();

	ev_func func;

	EventFunc0(ev_func f):EventBase(0)
	{
		func=f;
	}

	void call()
	{
		func();
	}
};

template<typename RT,typename P0>
struct EventFunc1:public EventBase
{
	typedef RT (*ev_func)(P0);

	ev_func func;
	RT result;
	P0 p0;

	EventFunc1(ev_func f)
	{
		func=f;
	}

	void set_param(int index,void *p)
	{
		if(index==0)p0=(P0)p;
	}

	void call()
	{
		result=func(p0);
	}

	void *get_result()
	{
		return result;
	}
};

template<typename RT,typename P0,typename P1>
struct EventFunc2:public EventBase
{
	typedef RT (*ev_func)(P0,P1);

	ev_func func;
	RT result;
	P0 p0;
	P1 p1;

	EventFunc2(ev_func f)
	{
		func=f;
	}

	void set_param(int index,void *p)
	{
		if(index==0)p0=(P0)p;else
		if(index==1)p1=(P1)p;
	}

	void call()
	{
		result=func(p0,p1);
	}

	void *get_result()
	{
		return result;
	}
};

template<typename RT																								>EventBase *CreateEvent(RT (*func)(							)){return(new EventFunc0<RT							>(func));}
template<typename RT,typename P0																					>EventBase *CreateEvent(RT (*func)(P0						)){return(new EventFunc1<RT,P0						>(func));}
template<typename RT,typename P0,typename P1																		>EventBase *CreateEvent(RT (*func)(P0,P1					)){return(new EventFunc2<RT,P0,P1					>(func));}
// template<typename RT,typename P0,typename P1,typename P2															>EventBase *CreateEvent(RT (*func)(P0,P1,P2					)){return(new EventFunc3<RT,P0,P1,P2				>(func));}
// template<typename RT,typename P0,typename P1,typename P2,typename P3												>EventBase *CreateEvent(RT (*func)(P0,P1,P2,P3				)){return(new EventFunc4<RT,P0,P1,P2,P3				>(func));}
// template<typename RT,typename P0,typename P1,typename P2,typename P3,typename P4									>EventBase *CreateEvent(RT (*func)(P0,P1,P2,P3,P4			)){return(new EventFunc5<RT,P0,P1,P2,P3,P4			>(func));}
// template<typename RT,typename P0,typename P1,typename P2,typename P3,typename P4,typename P5						>EventBase *CreateEvent(RT (*func)(P0,P1,P2,P3,P4,P5		)){return(new EventFunc6<RT,P0,P1,P2,P3,P4,P5		>(func));}
// template<typename RT,typename P0,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6			>EventBase *CreateEvent(RT (*func)(P0,P1,P2,P3,P4,P5,P6		)){return(new EventFunc7<RT,P0,P1,P2,P3,P4,P5,P6	>(func));}
// template<typename RT,typename P0,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7>EventBase *CreateEvent(RT (*func)(P0,P1,P2,P3,P4,P5,P6,P7	)){return(new EventFunc8<RT,P0,P1,P2,P3,P4,P5,P6,P7	>(func));}

template<typename RT>
RT Call(RT (*func)())
{
	return func();
}

template <typename RT, typename P1>
RT Call(RT (*func)(P1),P1 p1)
{
	return func(p1);
}

template <typename RT, typename P1, typename P2>
RT Call(RT (*func)(P1,P2),P1 p1,P2 p2)
{
	return func(p1,p2);
}

void zero()
{
	return;
}

int one()
{
	return 123;
}

void equ(int a)
{
}

int add(int a,int b)
{
	return(a+b);
}

class test
{
public:

	test()
	{
	}
};

template<typename RT,typename ...ARGS>
struct EventFuncAny
{
protected:

	typedef RT (*ev_func)(ARGS...);

	ev_func func;

public:

	EventFuncAny(ev_func f)
	{
		func=f;
	}

	void operator=(ev_func f)
	{
		func=f;
	}

	const int get_param_count()const{return(sizeof...(ARGS));}

	RT operator()(ARGS...args)
	{
		return func(args...);
	}

	RT call(ARGS...args)
	{
		return func(args...);
	}
};//

template<typename RT,typename ...ARGS>EventBase *CreateEventAny(RT (*func)(ARGS...))
{
	return(new EventFuncAny<RT,ARGS...>(func));
}

class BASE
{
public:

	int func(int a,int b)
	{
		return a+b;
	}
};

class NEWCLASS
{
public:

	int func(int a,int b)
	{
		return a*b;
	}
};

int main()
{
	int int_result;

	typedef int (BASE::*FP)(int,int);

	BASE b;

	FP fp=BASE::func;

	EventBase *ef_zero=CreateEvent(zero);
	EventBase *ef_one=CreateEvent(one);
//	EventBase *ef_equ=CreateEvent(equ);
//	EventBase *ef_add=CreateEvent(add);

	ef_zero->call();
	ef_one->call();
	ef_one->get_result(int_result);

	//ef_equ->set_param(0,0);
	//ef_equ->call();

	//ef_add->set_param(0,(void *)1);
	//ef_add->set_param(1,(void *)2);
	//ef_add->call();

	Call(zero);
	Call(equ,1);
	Call(add,1,2);

	EventFuncAny<void,int> any=equ;
	any(1);

	EventFuncAny<int> no_param=one;

	EventFuncAny<int,int,int> two_param=add;

	cout<<"any ParamCount="<<any.get_param_count()<<endl;
	cout<<"no_param ParamCount="<<no_param.get_param_count()<<",result="<<no_param()<<endl;
	cout<<"two_param ParamCount="<<two_param.get_param_count()<<",result="<<two_param(1,2)<<endl;

	return(0);
}
