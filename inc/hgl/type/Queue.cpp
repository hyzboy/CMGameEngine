#ifndef HGL_QUEUE_CPP
#define HGL_QUEUE_CPP

namespace hgl
{
    /**
    * 本类构造函数
    * @param m 如果m的值不为0，则表示使用固定的队列大小。固定大小的队列会在一开始即分配好内存。
    */
    template<typename T>
    Queue<T>::Queue(int m)
    {
        count=0;

        if(m)
        {
            max_count=m;

            items=hgl_aligned_malloc<T>(max_count);
        }
        else max_count=0;

        mem_count=max_count;
    }

    template<typename T>
    Queue<T>::~Queue()
    {
        if(count||max_count)hgl_free(items);
    }

    /**
    * 修改队列的最大值
    */
    template<typename T>
    void Queue<T>::SetMax(int m)
    {
        if(max_count||(!max_count&&count))
            items=(T *)hgl_realloc(items,m*sizeof(T));
        else
            items=hgl_aligned_malloc<T>(m);

        max_count=m;
        mem_count=m;

        if(count>=max_count)count=max_count-1;
    }

    /**
    * 清除队列中的所有数据
    */
    template<typename T>
    void Queue<T>::Clear()
    {
        if(max_count==0)
            if(count)
            {
                hgl_free(items);
                mem_count=0;
            }

        count=0;
    }

    /**
    * 清除队列中的所有数据，但不释放内存
    */
    template<typename T>
    void Queue<T>::ClearData()
    {
        count=0;
    }

    /**
    * 访问队列中的一个数据，但不清除它
    * @param t 取出的数据保存地
    * @return 是否成功取出数据
    */
    template<typename T>
    bool Queue<T>::Peek(T &t)
    {
        if(count)
        {
//          t=items[0];
            memcpy(&t,items,sizeof(T));
            return(true);
        }
        else
            return(false);
    }

    /**
     * 删除队列中的指定数据
     * @param index 索引
     */
    template<typename T>
    bool Queue<T>::Delete(int index)
    {
        if(index<0||index>=count)
            return(false);

        count--;

        if(count)
        {
            if(index<count)
                memmove(items+index,items+index+1,(count-index)*sizeof(T));
        }

        return(true);
    }

    /**
    * 从队列中取出一个数据
    * @param t 取出的数据保存地
    * @return 是否成功取出数据
    */
    template<typename T>
    bool Queue<T>::Pop(T &t)
    {
        if(count)
        {
//          t=items[0];
            memcpy(&t,items,sizeof(T));

            count--;

            if(max_count==0)
            {
                if(count)
                {
                    //memcpy(items,items+1,count*sizeof(T));
                    memmove(items,items+1,count*sizeof(T));
//                    items=(T *)hgl_realloc(items,count*sizeof(T));
                }
            }
            else
            {
                memcpy(items,items+1,count*sizeof(T));
            }

            return(true);
        }
        else
            return(false);
    }

    /**
    * 向队列中放入一个数据
    * @param data 要放入的数据指针
    * @return true 放入数据成功
    * @return false 放入数据失败
    */
    template<typename T>
    bool Queue<T>::Push(const T &data)
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

    template<typename T>
    int Queue<T>::Find(const T &data)
    {
        if(count<=0)
            return(-1);

        T *p=items;
        for(int i=0;i<count;i++)
        {
            if(*p==data)
                return(i);

            ++p;
        }

        return(-1);
    }

    template<typename T>
    void Queue<T>::operator =(const Queue<T> &ori)
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
}

namespace hgl
{
    template<typename T>
    void QueueObject<T>::Clear()
    {
        int n=Queue<T *>::count;

        while(n--)
            delete Queue<T *>::items[n];

        Queue<T *>::Clear();
    }
}
#endif//HGL_QUEUE_CPP
