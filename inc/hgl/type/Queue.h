#ifndef HGL_QUEUE_INCLUDE
#define HGL_QUEUE_INCLUDE

#include<hgl/type/DataType.h>
namespace hgl
{
    /**
    * Queue模板类用于保存一个先进先出、后进后出的数据队列
    *
    * 注：这个类还在测试中，请不要修改这个类的源代码，如有修改意见，请致电作者。
    */
    template<typename T> class Queue                                                                ///队列顺序数据访问类
    {
    protected:

        int max_count;
        int mem_count;
        int count;
        T *items;

    public: //属性

        T *GetData()const{return items;}                                                            ///<取得原始数据
        int GetCount()const{return count;}                                                          ///<取得数据数量

        int GetMax()const{return max_count;}                                                        ///<取得最大数量
        void SetMax(int);                                                                           ///<设置最大数量

        T *GetData(){return items;}                                                                 ///<取得原始数据

    public: //方法

        Queue(int=0);
        virtual ~Queue();

        bool Peek(T &);                                                                             ///<尝试访问一个数据
        bool Pop(T &);                                                                              ///<弹出一个数据
        bool Push(const T &);                                                                       ///<压入一个数据

        int Find(const T &);                                                                        ///<查找队列中这个数据的编号
        bool Delete(int);                                                                           ///<删除队列中指定编号的数据

        void Clear();                                                                               ///<清除所有数据
        void ClearData();                                                                           ///<清除所有数据，但不释放内存

        bool GetItem(int n,T &ti)                                                                   ///<取得指定项数据
        {
            if(n<0||n>=count)return(false);

            ti=items[n];
            return(true);
        }

        virtual void operator =(const Queue<T> &);
    };//template<typename T> class Queue

    template<typename T> class QueueObject:public Queue<T *>                                        ///堆栈对象
    {
    public:

        using Queue<T *>::Queue;
        virtual ~QueueObject(){Clear();}

        void Clear();

        T *operator[](int n)const
        {
            if(n<0||n>=Queue<T *>::count)return(nullptr);

            return Queue<T *>::items[n];
        }
    };//template<typename T> class QueueObject
}//namespace hgl
#include<hgl/type/Queue.cpp>
#endif//HGL_QUEUE_INCLUDE
