#ifndef HGL_FIXEDLIST_CPP
#define HGL_FIXEDLIST_CPP

#include<hgl/type/FixedList.h>
#include<string.h>

#ifdef _DEBUG
#include<hgl/LogInfo.h>
#endif//_DEBUG
namespace hgl
{
    template<typename T>
    void FixedList<T>::InitPrivate()
    {
        count=0;
        max_count=0;
        items=nullptr;

        OnClear=nullptr;
    }

    template<typename T>
    FixedList<T>::FixedList()
    {
        InitPrivate();
    }

    template<typename T>
    FixedList<T>::FixedList(int value)
    {
        InitPrivate();

        Create(value);
    }

    template<typename T>
    FixedList<T>::~FixedList()
    {
        if(items)
            hgl_free(items);
    }

    template<typename T>
    void FixedList<T>::Create(int value)
    {
        if(items)
            hgl_free(items);

        if(value>0)
        {
            max_count=value;

            items=hgl_aligned_malloc<T>(max_count);

            memset(items,0,max_count*sizeof(T));
        }
    }

    template<typename T>
    bool FixedList<T>::Get(int n,T &td)
    {
        if(n<0||n>=count)
        {
            LOG_ERROR(OS_TEXT("FixedList<T>::Get 数据索引超出正常范围：index=")+OSString(n)+OS_TEXT("count=")+OSString(count));
            return(false);
        }

        td=items[n];
        return(true);
    }

    template<typename T>
    bool FixedList<T>::Append(const T &data)
    {
        if(count>=max_count)
        {
            LOG_ERROR(OS_TEXT("FixedList<T>::Append(data); 数据已满！"));
            return(false);
        }

        //items[count++]=data;
        memcpy(items+count,&data,sizeof(T));
        count++;

        return(true);
    }

    template<typename T>
    bool FixedList<T>::Append(const T *data,int n)
    {
        if(count+n>max_count)
        {
            LOG_ERROR(OS_TEXT("FixedList<T>::Append(data,n); 数据已满！"));
            return(false);
        }

        memcpy(items+count,data,n*sizeof(T));
        count+=n;
        return(true);
    }

    template<typename T>
    int  FixedList<T>::Find(const T &data)
    {
        int n=count;
        T *p=items+n-1;

        while(n--)
        {
            if((*p--)==data)
                return n;
        }

        return -1;
    }

    template<typename T>
    bool FixedList<T>::Delete(int n)
    {
        if(n<0||n>=count)
        {
            LOG_ERROR(OS_TEXT("FixedList<T>::Delete 数据索引超出正常范围：index=")+OSString(n)+OS_TEXT("count=")+OSString(count));
            return(false);
        }

        if(OnClear)
            OnClear(items[n]);

        count--;

        if(count)
        {
            if(n<count)                 //将最后一个数据移到当前位置
                //items[n]=items[count];
                memcpy(items+n,items+count,sizeof(T));
        }

        return(true);
    }

    template<typename T>
    void FixedList<T>::Delete(const T *data,int n)
    {
        while(n--)
        {
            int index=Find(*data);

            if(index!=-1)
                Delete(index);

            data++;
        }
    }

    template<typename T>
    void FixedList<T>::Clear()
    {
        if(OnClear)
        {
            int n=count;

            while(n--)
                OnClear(items[n]);
        }

        memset(items,0,max_count*sizeof(T));
        count=0;
    }

    template<typename T>
    T *FixedList<T>::CreateCopy(int &c)
    {
        c=count;
        T *copy=hgl_aligned_malloc<T>(count);
        memcpy(copy,items,count*sizeof(T));

        return copy;
    }
}//namespace hgl

namespace hgl
{
    template<typename T>
    bool FixedObjectList<T>::Delete(int n)
    {
        if(n<0||n>=FixedList<T>::count)
        {
            LOG_ERROR(OS_TEXT("FixedObjectList<T>::Delete[] 数据索引超出正常范围：index=")+OSString(n)+OS_TEXT("count=")+OSString(FixedList<T>::count));
            return(false);
        }

        delete FixedList<T>::items[n];

        FixedList<T>::count--;

        if(FixedList<T>::count)
        {
            if(n<FixedList<T>::count)                   //将最后一个数据移到当前位置
                //items[n]=items[count];
                memcpy(FixedList<T>::items+n,FixedList<T>::items+FixedList<T>::count,sizeof(T));
        }

        return(true);
    }

    template<typename T>
    void FixedObjectList<T>::Clear()
    {
        int n=FixedList<T>::count;

        while(n--)
            delete FixedList<T>::items[n];        //delete NULL;不会出错,所以不必检测

        FixedList<T *>::Clear();
    }
}//namespace hgl

namespace hgl
{
    template<typename T>
    bool MTFixedObjectList<T>::Delete(int n)
    {
        if(n<0||n>=FixedList<T>::count)
        {
            LOG_ERROR(OS_TEXT("MTFixedObjectList<T>::Delete 数据索引超出正常范围：index=")+OSString(n)+OS_TEXT("count=")+OSString(FixedList<T>::count));
            return(false);
        }

        delete FixedList<T>::items[n];

        FixedList<T>::count--;

        if(FixedList<T>::count)
        {
            if(n<FixedList<T>::count)                   //将最后一个数据移到当前位置
                //items[n]=items[count];
                memcpy(FixedList<T>::items+n,FixedList<T>::items+FixedList<T>::count,sizeof(T));
        }

        return(true);
    }

    template<typename T>
    void MTFixedObjectList<T>::Clear()
    {
        int n=FixedList<T>::count;

        while(n--)
            delete FixedList<T>::items[n];        //delete NULL;不会出错,所以不必检测

        FixedList<T *>::Clear();
    }
}//namespace hgl
#endif//HGL_FIXEDLIST_CPP
