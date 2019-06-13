#ifndef HGL_RAND_SELECT_CPP
#define HGL_RAND_SELECT_CPP

#include<hgl/type/RandSelect.h>
#include<hgl/Other.h>
namespace hgl
{
    template<typename T>
    void RandSelect<T>::InitPrivate()
    {
        hglSetProperty(MinValue,this,RandSelect<T>::GetMin,RandSelect<T>::SetMin);
        hglSetProperty(MaxValue,this,RandSelect<T>::GetMax,RandSelect<T>::SetMax);
    }

    template<typename T>
    RandSelect<T>::RandSelect()
    {
        InitPrivate();
    }

    /**
    * 本类构造函数
    * @param s 最小值
    * @param l 最大值
    * @param p 精度
    */
    template<typename T>
    RandSelect<T>::RandSelect(T s,T l,T p)
    {
        InitPrivate();

        Set(s,l,p);
    }

    /**
    * 设置随机数产生条件
    */
    template<typename T>
    void RandSelect<T>::Set(T s,T l,T p)
    {
        min_value=s;
        max_value=l;
        pre=p;

        gap=(max_value-min_value)/pre+1;
    }

    /**
    * 产生一个随机数
    */
    template<typename T>
    T RandSelect<T>::Select()
    {
        return(min_value+(FastRand()%gap)*pre);
    }

    template<typename T>
    void RandSelect<T>::SetMin(T s)
    {
        if(s>max_value)
        {
            min_value=max_value;
            max_value=s;
        }
        else
            min_value=s;

        gap=(max_value-min_value)/pre+1;
    }

    template<typename T>
    void RandSelect<T>::SetMax(T l)
    {
        if(l<min_value)
        {
            max_value=min_value;
            min_value=l;
        }
        else
            max_value=l;

        gap=(max_value-min_value)/pre+1;
    }
}//namespace hgl
#endif//HGL_RAND_SELECT_CPP
