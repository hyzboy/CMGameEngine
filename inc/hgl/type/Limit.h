#ifndef HGL_LIMIT_INCLUDE
#define HGL_LIMIT_INCLUDE

#include<hgl.h>
namespace hgl
{
	/**
	* 限制型数据模板类,这个模板可以定义值的上下限。这样在对数值进行赋值时就无需注意上下限了。
	* 但需要注意的是使用Limit运行速度比手动写判断要慢。
	*/
	template<class T> class Limit                                                                   ///限制型数据模板类
	{
		T _value;
		T _min,_max;

		void setvalue(T v)
		{
			if(v<_min)_value=_min;else
			if(v>_max)_value=_max;else _value=v;
		}

		T getmin(){return _min;}
		T getmax(){return _max;}
		void setmin(T v){_min=v;if(_value<_min)_value=_min;}
		void setmax(T v){_max=v;if(_value>_max)_value=_max;}

		bool ismin(){return(_value==_min);}
		bool ismax(){return(_value==_max);}

		void InitPrivate(T,T);

	public:

		Property<T> Min;                                                                            ///<最小值虚拟变量
		Property<T> Max;                                                                            ///<最大值虚拟变量

		Property<bool> IsMin;																		///<是否是最小值
		Property<bool> IsMax;																		///<是否是最大值

	public:

		Limit(){InitPrivate(0,0);}
		Limit(T i,T a){InitPrivate(i,a);}

		operator T () const{return(_value);}

		void operator = (T v){setvalue(v);}

		void Set(T i,T a,T c){_min=i;_max=a;_value=c;}
		void ToMin(){_value=_min;}
		void ToMax(){_value=_max;}

		T operator + (T v){return(_value+v);}
		T operator - (T v){return(_value-v);}
		T operator * (T v){return(_value*v);}
		T operator / (T v){return(_value/v);}
		T operator % (T v){return(_value%v);}

		void operator += (T v){setvalue(_value+v);}
		void operator -= (T v){setvalue(_value-v);}
		void operator *= (T v){setvalue(_value*v);}
		void operator /= (T v){setvalue(_value/v);}

		T operator ++ (){setvalue(_value++);return(_value);}
		T operator -- (){setvalue(_value--);return(_value);}

		bool operator > (T v){return(_value> v);}
		bool operator >=(T v){return(_value>=v);}
		bool operator < (T v){return(_value< v);}
		bool operator <=(T v){return(_value<=v);}

		bool operator ==(T v){return(_value==v);}
		bool operator !=(T v){return(_value!=v);}
	};//template<class T> class Limit
}//namespace hgl
#include<hgl/type/Limit.cpp>
#endif//HGL_LIMIT_INCLUDE
