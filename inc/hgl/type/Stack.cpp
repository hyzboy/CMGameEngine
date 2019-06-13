#ifndef HGL_STACK_CPP
#define HGL_STACK_CPP

#include<hgl/LogInfo.h>
namespace hgl
{
    /**
    * 本类构造函数
    * @param m 如果m的值不为0，则表示使用固定的堆栈大小。固定大小的堆栈会在一开始即分配好内存。
    */
    template<typename T>
    Stack<T>::Stack(int m)
    {
        count=0;

        if(m)
        {
            max_count=m;

            items=hgl_aligned_malloc<T>(max_count);
        }
        else
            max_count=0;

        mem_count=max_count;
    }

    template<typename T>
    Stack<T>::~Stack()
    {
        if(count||max_count)hgl_free(items);
    }

    /**
    * 修改堆栈的最大值
    */
    template<typename T>
    void Stack<T>::SetMax(int m)
    {
        if(m<=0)return;

        if(mem_count==0)
        {
            mem_count=power_to_2(m);
            items=hgl_aligned_malloc<T>(mem_count);
        }
        else
        if(mem_count<m)
        {
            mem_count=power_to_2(m);
            items=(T *)hgl_realloc(items,mem_count*sizeof(T));
        }

        max_count=m;

        if(count>=max_count)count=max_count-1;
    }

    template<typename T>
    bool Stack<T>::SetCount(int c)
    {
        if(c<0)return(false);

        if(c>max_count)
            return(false);

        count=c;
        return(true);
    }

    /**
    * 清除堆栈中的所有数据
    */
    template<typename T>
    void Stack<T>::Clear()
    {
        if(max_count==0)
            if(count)
            {
                hgl_free(items);
                mem_count=0;
            }

        count=0;
    }

    template<typename T>
    bool Stack<T>::GetItem(int n,T &data)
    {
        if(n<0||n>=count)
        {
            LOG_ERROR(OS_TEXT("从堆栈中按索引<") + OSString(n) + OS_TEXT(">取数据，超出正常范围<")+OSString(count) + OS_TEXT(">"));

            return(false);
        }

        data=items[n];
        return(true);
    }

    /**
    * 访问堆栈中的一个数据，但不清除它
    * @param t 取出的数据保存地
    * @return 是否成功取出数据
    */
    template<typename T>
    bool Stack<T>::Peek(T &t)
    {
        if(count)
        {
            memcpy(&t,items+(count-1),sizeof(T));
//          t=items[count-1];
            return(true);
        }
        else
            return(false);
    }

    /**
    * 从堆栈中取出一个数据
    * @param t 取出的数据保存地
    * @return 是否成功取出数据
    */
    template<typename T>
    bool Stack<T>::Pop(T &t)
    {
        if(count)
        {
//          t=items[--count];
            count--;
            memcpy(&t,items+count,sizeof(T));

            if(max_count==0)
            {
                if(count==0)
                {
                    hgl_free(items);

                    mem_count=0;
                }
                //else
                    //items=(T *)hgl_realloc(items,count*sizeof(T));
            }

            return(true);
        }
        else
            return(false);
    }

    /**
    * 向堆栈中放入一个数据
    * @param data 要放入的数据指针
    * @return true 放入数据成功
    * @return false 放入数据失败
    */
    template<typename T>
    bool Stack<T>::Push(T &data)
    {
        if(max_count)
        {
            if(count>=max_count)return(false);
        }
        else
        {
            if(count)
            {
                if(count+1>mem_count)
                {
                    mem_count=power_to_2(count+1);

                    items=(T *)hgl_realloc(items,mem_count*sizeof(T));
                }
            }
            else
            {
                items=hgl_aligned_malloc<T>(1);

                mem_count=1;
            }
        }

//      items[count++]=data;
        memcpy(items+count,&data,sizeof(T));
        count++;

        return(true);
    }

    /**
    * 向堆栈中放入多个数据
    * @param data 要放入的数据
    * @param data_count 要放入的数据个数
    * @return true 放入数据成功
    * @return false 放入数据失败
    */
    template<typename T>
    bool Stack<T>::MultiPush(T *data,int data_count)
    {
        if(max_count)
        {
            if(count>=max_count)return(false);
        }
        else
        {
            if(count)
            {
                if(count+data_count>mem_count)
                {
                    mem_count=power_to_2(count+data_count);

                    items=(T *)hgl_realloc(items,mem_count*sizeof(T));
                }
            }
            else
            {
                items=hgl_aligned_malloc<T>(data_count);

                mem_count=data_count;
            }
        }

        memcpy(items+count,data,data_count*sizeof(T));
        count+=data_count;

        return(true);
    }

    template<typename T>
    void Stack<T>::operator =(const Stack<T> &ori)
    {
        if(ori.count==0)return;

        Clear();

        max_count=ori.count;
        count=ori.count;

        if(max_count==0)
            mem_count=count;
        else
            mem_count=max_count;

        items=hgl_aligned_malloc<T>(mem_count);

        memcpy(items,ori.items,mem_count*sizeof(T));
    }
}//namespace hgl

namespace hgl
{
    template<typename T>
    void StackObject<T>::Clear()
    {
        for(int i=0;i<this->count;i++)
            delete this->items[i];

        Stack<T *>::Clear();
    }
}//namespace hgl
#endif//HGL_STACK_CPP
