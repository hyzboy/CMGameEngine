#ifndef HGL_THREAD_ATOMIC_WINDOWS_INCLUDE
#define HGL_THREAD_ATOMIC_WINDOWS_INCLUDE

#include<windows.h>
namespace hgl
{
    //开发日志 2013-06-19
    //1.原本是一个模板，但将32/64中不一样的部分各自特例化，但不知为何VC2012中无法认出对应的操作符，所以改成2个模板
    //2.部分原子函数没有8/16位版本

    //32位版
    template<typename T> class atom_win32
    {
        volatile T value;

    public:

        atom_win32(){value=0;}
        atom_win32(const volatile T new_value){operator=(new_value);}

                    T       operator  *=        (const volatile T v)        {return operator=(value *=v);}
                    T       operator  /=        (const volatile T v)        {return operator=(value /=v);}
                    T       operator  %=        (const volatile T v)        {return operator=(value %=v);}
                    T       operator >>=        (const volatile T v)        {return operator=(value>>=v);}
                    T       operator <<=        (const volatile T v)        {return operator=(value<<=v);}

                    T       operator !          ()const                     {return !value;}
                    T       operator ~          ()const                     {return ~value;}

                            operator T          ()                          {return value;}
                            operator const T    ()const                     {return value;}
                    bool    operator !          ()                          {return !value;}

                    bool    operator ==         (const volatile T v)const   {return value==v;}
                    bool    operator !=         (const volatile T v)const   {return value!=v;}

                    T       operator =          (const volatile T nv)       {return InterlockedExchange((unsigned long *)&value,(unsigned long)nv);}
                    T       operator ++         ()                          {return InterlockedIncrement((unsigned long *)&value);}
                    T       operator --         ()                          {return InterlockedDecrement((unsigned long *)&value);}
                    T       operator +=         (const volatile T av)       {return InterlockedExchangeAdd((unsigned long *)&value, av); }
                    T       operator -=         (const volatile T av)       {return InterlockedExchangeAdd((unsigned long *)&value, -av); }

        volatile    T       operator &=         (const volatile T v)        {return InterlockedAnd((unsigned long *)&value,v);}
        volatile    T       operator |=         (const volatile T v)        {return InterlockedOr((unsigned long *)&value, v); }
        volatile    T       operator ^=         (const volatile T v)        {return InterlockedXor((unsigned long *)&value, v); }

        volatile    T       operator ++         (int)                       {volatile T ret=value;operator++();return ret;}//后置++
        volatile    T       operator --         (int)                       {volatile T ret=value;operator--();return ret;}//后置--
    };//template<typename T> class atom_win32

    //64位版
    template<typename T> class atom_win64
    {
        volatile T value;

    public:

        atom_win64(){value=0;}
        atom_win64(const volatile T new_value){operator=(new_value);}

                    T       operator  *=        (const volatile T v)        {return operator=(value *=v);}
                    T       operator  /=        (const volatile T v)        {return operator=(value /=v);}
                    T       operator  %=        (const volatile T v)        {return operator=(value %=v);}
                    T       operator >>=        (const volatile T v)        {return operator=(value>>=v);}
                    T       operator <<=        (const volatile T v)        {return operator=(value<<=v);}

                    T       operator !          ()const                     {return !value;}
                    T       operator ~          ()const                     {return ~value;}

                            operator T          ()                          {return value;}
                            operator const T    ()const                     {return value;}
                    bool    operator !          ()                          {return !value;}

                    bool    operator ==         (const volatile T v)const   {return value==v;}
                    bool    operator !=         (const volatile T v)const   {return value!=v;}

                    T       operator =          (const volatile T nv)       {return InterlockedExchange64((LONG64 *)&value, (LONG64)nv); }
                    T       operator ++         ()                          {return InterlockedIncrement64((LONG64 *)&value); }
                    T       operator --         ()                          {return InterlockedDecrement64((LONG64 *)&value); }
                    T       operator +=         (const volatile T av)       {return InterlockedExchangeAdd64((LONG64 *)&value, av); }
                    T       operator -=         (const volatile T av)       {return InterlockedExchangeAdd64((LONG64 *)&value, -av); }

        volatile    T       operator &=         (const volatile T v)        {return InterlockedAnd64((LONG64 *)&value, v); }
        volatile    T       operator |=         (const volatile T v)        {return InterlockedOr64((LONG64 *)&value, v); }
        volatile    T       operator ^=         (const volatile T v)        {return InterlockedXor64((LONG64 *)&value, v); }

        volatile    T       operator ++         (int)                       {volatile T ret=value;operator++();return ret;}//后置++
        volatile    T       operator --         (int)                       {volatile T ret=value;operator--();return ret;}//后置--
    };//template<typename T> class atom_win64
}//namespace hgl
#endif//HGL_THREAD_ATOMIC_WINDOWS_INCLUDE
