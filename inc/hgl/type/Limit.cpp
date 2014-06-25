#ifndef HGL_LIMIT_CPP
#define HGL_LIMIT_CPP

#include<hgl/type/Limit.h>
namespace hgl
{
	template<class T>
	void Limit<T>::InitPrivate(T i,T a)
	{
		if(i<a)
		{
			_min=i;
			_max=a;
		}
		else
		{
			_min=a;
			_max=i;
		}

		_value=_min;

		hglSetProperty(Min,this,Limit<T>::getmin,Limit<T>::setmin);
		hglSetProperty(Max,this,Limit<T>::getmax,Limit<T>::setmax);
		hglSetPropertyRead(IsMin,this,Limit<T>::ismin);
		hglSetPropertyRead(IsMax,this,Limit<T>::ismax);
	}
}//namespace hgl
#endif//HGL_LIMIT_CPP
