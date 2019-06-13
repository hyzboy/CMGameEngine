#ifndef HGL_STACK_INCLUDE
#define HGL_STACK_INCLUDE

#include<hgl/type/DataType.h>
namespace hgl
{
    /**
    * Stack模板类用于保存一个先进后出、后进先出的数据堆栈
    *
    * 注：这个类还在测试中，请不要修改这个类的源代码，如有修改意见，请致电作者。
    */
    template<typename T> class Stack                                                                ///堆栈顺序数据访问类
    {
    protected:

        int max_count;
        int mem_count;
        int count;
        T *items;

    public: //属性

        int   GetCount()      const{return count;}                                                  ///<取得堆栈中数据的个数
        bool  SetCount(int c);                                                                      ///<直接设置堆栈中数据的个数

        int   GetMax  ()      const{return max_count;}                                              ///<取得堆栈中的最大数据个数
        void  SetMax  (int);                                                                        ///<设置堆栈中的最大数据个数

        T *   GetData ()           {return items;}                                                  ///<取得原始数据

    public: //方法

        Stack(int=0);
        virtual ~Stack();

                bool Peek(T &);                                                                     ///<尝试访问一个数据
        virtual bool Pop(T &);                                                                      ///<弹出一个数据
                bool Push(T &);                                                                     ///<压入一个数据
                bool MultiPush(T *,int);                                                            ///<放入多个数据

        virtual void Clear();                                                                       ///<清除所有数据

                bool GetItem(int,T &);

        virtual void operator =(const Stack<T> &);
    };//template<typename T> class Stack

    template<typename T> class StackObject:public Stack<T *>                                        ///堆栈对象
    {
    public:

        using Stack<T *>::Stack;
        virtual ~StackObject(){Clear();};

        void Clear();
    };//template<typename T> class StackObject
}//namespace hgl
#include<hgl/type/Stack.cpp>
#endif//HGL_STACK_INCLUDE
