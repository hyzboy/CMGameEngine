#include<iostream>
#include <project/Common/Character.h>

using namespace std;

typedef char        int8;
typedef short       int16;
typedef int         int32;
typedef long long   int64;

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;

typedef u16char * UTF16String;
//--------------------------------------------------------------------------------------------------
enum DataTypeEnum
{
    dteVoid=0,

    dteInt8,
    dteInt16,
    dteInt32,
    dteInt64,

    dteUint8,
    dteUint16,
    dteUint32,
    dteUint64,

    dteFloat,
    dteDouble,

    dteString,
};

template<typename T> struct TypeParse
{
    DataTypeEnum GetType();
};

template<> struct TypeParse<void        >{DataTypeEnum GetType(){return dteVoid;    }};

template<> struct TypeParse<int8        >{DataTypeEnum GetType(){return dteInt8;    }};
template<> struct TypeParse<int16       >{DataTypeEnum GetType(){return dteInt16;   }};
template<> struct TypeParse<int32       >{DataTypeEnum GetType(){return dteInt32;   }};
template<> struct TypeParse<int64       >{DataTypeEnum GetType(){return dteInt64;   }};

template<> struct TypeParse<uint8       >{DataTypeEnum GetType(){return dteUint8;   }};
template<> struct TypeParse<uint16      >{DataTypeEnum GetType(){return dteUint16;  }};
template<> struct TypeParse<uint32      >{DataTypeEnum GetType(){return dteUint32;  }};
template<> struct TypeParse<uint64      >{DataTypeEnum GetType(){return dteUint64;  }};

template<> struct TypeParse<float       >{DataTypeEnum GetType(){return dteFloat;   }};
template<> struct TypeParse<double      >{DataTypeEnum GetType(){return dteDouble;  }};
template<> struct TypeParse<UTF16String  >{DataTypeEnum GetType(){return dteString;  }};

//--------------------------------------------------------------------------------------------------
template<typename R>
struct FuncCall
{
    R (*func)();

    FuncCall()
    {
        func=nullptr;
    }

    R operator()()
    {
        return func();
    }
};

template<> struct FuncCall<void>
{
    void (*func)();

    void operator()()
    {
        func();
    }
};

void test()
{
    cout<<"void test();"<<endl;
}

int test_ri()
{
    cout<<"int test_ri();"<<endl;
}
//--------------------------------------------------------------------------------------------------
/**
* 最终基类,无任何作用
*/
class _Object                                                                                       ///最终基类
{
public:

    virtual ~_Object(){}                                                                            ///<本类析构函数
};//class _Object

typedef void (_Object::*ObjectMemberFunc)();

//此代码取自AngelScript，感谢
#define GetMemberFuncPointer(c,m) MethodPtr<sizeof(void (c::*)())>::Convert((void (c::*)())(&c::m))

template <int> struct MethodPtr
{
    template<class M>
    static void *Convert(M Mthd)
    {
        union u
        {
            M m;
            void *v;
        };

        return ((u *)(&Mthd))->v;
    }
};//struct MethodPtr

template<typename R,typename F> struct EventFunc
{
    union
    {
        void *vp_this;
        _Object *this_pointer;
    };

    union
    {
        void *vp_func;
        F func_pointer;
        ObjectMemberFunc omf;
    };

    TypeParse<R> ReturnType;            //返回数据类型

    virtual int GetArgCount()=0;        //取得参数个数
    virtual bool SetArg(int,void *)=0;  //设置参数

    virtual void _Call()=0;             //呼叫事件

            bool Call()
    {
        if(!this_pointer)return(false);
        if(!func_pointer)return(false);

        return _Call();
    }
};

#define DefEvent(result,name,intro)                                 EventFunc<result,result (_Object:: *)intro> name;

#define SetEventCall(event_obj,obj_this,class_name,event_func)      {   \
                                                                        event_obj.vp_this=obj_this; \
                                                                        event_obj.vp_func=GetMemberFuncPointer(class_name,event_func);  \
                                                                    }

template<typename R> struct EventFunc0:public EventFunc<R,R (_Object:: *)()>
{
    R operator()()
    {
        return (this->this_pointer->*(this->func_pointer))();
    }

    R ret;

    int GetArgCount()const{return 0;}
    bool SetArg(int,void *){return false;}

    void _Call()
    {
        ret=operator()();
    }
};//template<typename R> struct EventFuncArg0

template<> struct EventFunc0<void>:public EventFunc<void,void (_Object:: *)()>
{
    void operator()()
    {
        (this->this_pointer->*(this->func_pointer))();
    }

    int GetArgCount()const{return 0;}
    bool SetArg(int,void *){return false;}

    void _Call()
    {
        operator()();
    }
};

template<typename R,typename A0> struct EventFunc1:public EventFunc<R,R (_Object:: *)(A0)>
{
    R operator()(A0 a0)
    {
        return (this->this_pointer->*(this->func_pointer))(a0);
    }

    R ret;
    A0 arg0;

    int GetArgCount()const{return 1;}
    bool SetArg(int index,void *val)
    {
        if(index)return(false);

        arg0=(A0)val;
        return(true);
    }

    void _Call()
    {
        ret=operator()(arg0);
    }
};//

template<typename A0> struct EventFunc1<void,A0>:public EventFunc<void,void (_Object:: *)(A0)>
{
    void operator()(A0 a0)
    {
        (this->this_pointer->*(this->func_pointer))(a0);
    }

    A0 arg0;

    int GetArgCount()const{return 1;}
    bool SetArg(int index,void *val)
    {
        if(index)return(false);

        arg0=(A0)val;
        return(true);
    }

    void _Call()
    {
        operator()(arg0);
    }
};

template<typename R,typename A0,typename A1> struct EventFunc2:public EventFunc<R,R (_Object:: *)(A0,A1)>
{
    A0 arg0;
    A1 arg1;

    R operator()(A0 a0,A1 a1)
    {
        return (this->this_pointer->*(this->func_pointer))(a0,a1);
    }
};//

template<typename A0,typename A1> struct EventFunc2<void,A0,A1>:public EventFunc<void,void (_Object:: *)(A0,A1)>
{
    void operator()(A0 a0,A1 a1)
    {
        (this->this_pointer->*(this->func_pointer))(a0,a1);
    }
};

class TestClass
{
public:

    TestClass()
    {
    }

    void test0()
    {
        cout<<"void TestClass::test();"<<endl;
    }

    void test1(int a)
    {
        cout<<"void TestClass::test1(int="<<a<<");"<<endl;
    }

    int test11(int a)
    {
        cout<<"int TestClass::test11(int="<<a<<");\treturn "<<a<<"*2;"<<endl;

        return a*2;
    }

    void test2(int a,int b)
    {
        cout<<"void TestClass::test2(int="<<a<<",int="<<b<<");"<<endl;
    }

    int test22(int a,int b)
    {
        cout<<"int TestClass::test22(int="<<a<<",int="<<b<<");\treturn "<<a<<"+"<<b<<";"<<endl;

        return a+b;
    }
};//class TestClass

int main(int,char **)
{
    FuncCall<void> fp;

    fp.func=test;
    fp();

    FuncCall<int> fp_ri;

    fp_ri.func=test_ri;
    fp_ri();

    EventFunc0<void> OnTest0;
    EventFunc1<void,int> OnTest1;
    EventFunc1<int,int> OnTest11;
    EventFunc2<void,int,int> OnTest2;
    EventFunc2<int,int,int> OnTest22;

    TestClass tc;

    SetEventCall(OnTest0    ,&tc,TestClass,test0);
    SetEventCall(OnTest1    ,&tc,TestClass,test1);
    SetEventCall(OnTest11   ,&tc,TestClass,test11);

    SetEventCall(OnTest2    ,&tc,TestClass,test2);
    SetEventCall(OnTest22   ,&tc,TestClass,test22);

    OnTest0();
    OnTest1(123);
    cout<<"OnTest11(256) return "<<OnTest11(256)<<";"<<endl;

    OnTest2(123,456);
    cout<<"OnTest22(123,256) return "<<OnTest22(123,256)<<";"<<endl;

    return(0);
}
