#ifndef HGL_TYPE_SET_CPP
#define HGL_TYPE_SET_CPP

#include<hgl/type/Set.h>
namespace hgl
{
    /**
    * 查找数据是否存在
    * @param flag 数据
    * @return 数据所在索引，-1表示不存在
    */
    template<typename T>
    const int Set<T>::Find(const T &flag)const
    {
        int left=0,right=data_list.GetCount()-1;                //使用left,right而不使用min,max是为了让代码能够更好的阅读。
        int mid;

        T *data_array=data_list.GetData();

        while(left<=right)
        {
            if(data_array[left ]==flag)return(left);
            if(data_array[right]==flag)return(right);

            mid=(right+left)>>1;

            if(data_array[mid]==flag)return(mid);

            if(data_array[mid]>flag)
            {
                left++;
                right=mid-1;
            }
            else
            {
                right--;
                left=mid+1;
            }
        }

        return(-1);
    }

    template<typename T>
    bool Set<T>::FindPos(const T &flag,int &pos)const
    {
        int left=0,right=data_list.GetCount()-1;
        int mid;

        T *data_array=data_list.GetData();

        while(left<=right)
        {
            if(data_array[left]>flag)
            {
                pos=left;
                return(false);
            }
            else
            if(data_array[left]==flag)
            {
                pos=left;
                return(true);
            }

            if(data_array[right]<flag)
            {
                pos=right+1;
                return(false);
            }
            else
            if(data_array[right]==flag)
            {
                pos=right;
                return(true);
            }

            mid=(right+left)>>1;

            if(data_array[mid]==flag)
            {
                pos=mid;
                return(true);
            }

            if(data_array[mid]>flag)
            {
                if(data_array[mid-1]<flag)
                {
                    pos=mid;
                    return(false);
                }
                else
                if(data_array[mid-1]==flag)
                {
                    pos=mid-1;
                    return(true);
                }

                ++left;
                right=mid-1;
            }
            else
            {
                if(data_array[mid+1]>flag)
                {
                    pos=mid+1;
                    return(false);
                }
                else
                if(data_array[mid+1]==flag)
                {
                    pos=mid+1;
                    return(true);
                }

                --right;
                left=mid+1;
            }
        }

        return(false);
    }

    /**
    * 添加一个数据
    * @param data 数据
    * @return 位置
    */
    template<typename T>
    int Set<T>::Add(const T &data)
    {
        if(data_list.GetCount()<=0)
        {
            data_list.Add(data);

            return 0;
        }
        else
        {
            int pos;

            if(FindPos(data,pos))
                return(-1);         //数据已存在

            data_list.Insert(pos,data);

            return(pos);
        }
    }

    /**
    * 添加一批数据
    * @param dp 数据指针
    * @param count 数据个数
    * @return 成功加入的数据个数
    */
    template<typename T>
    int Set<T>::Add(const T *dp,const int count)
    {
        int total=0;

        for(int i=0;i<count;i++)
        {
            if(Add(*dp)!=-1)
                ++total;

            ++dp;
        }

        return total;
    }

    /**
     * 更新一个数据
     * @param data 数据
     * @return 是否成功
     */
    template<typename T>
    bool Set<T>::Update(const T &data)
    {
        if(data_list.GetCount()<=0)
            return(false);

        int pos;

        if(!FindPos(data,pos))
            return(false);

        data_list.Set(pos,data);
        return(true);
    }

    /**
    * 删除一个数据
    * @param pos 索引编号
    */
    template<typename T>
    bool Set<T>::DeleteBySerial(int pos)
    {
        if(pos<0||pos>=data_list.GetCount())return(false);

        return data_list.DeleteMove(pos);
    }

    /**
    * 删除一个数据
    * @param data 数据
    */
    template<typename T>
    bool Set<T>::Delete(const T &data)
    {
        int pos=Find(data);

        if(pos==-1)return(false);

        return DeleteBySerial(pos);
    }

    /**
    * 删除一批数据
    * @param dp 数据指针
    * @param count 数据个数
    * @return 成功删除的数据个数
    */
    template<typename T>
    int Set<T>::Delete(T *dp,const int count)
    {
        int total=0;
        int pos;

        for(int i=0;i<count;i++)
        {
            pos=Find(*dp);
            if(pos!=-1)
            {
                DeleteBySerial(pos);
                ++total;
            }

            ++dp;
        }

        return total;
    }

    /**
    * 清除所有数据
    */
    template<typename T>
    void Set<T>::Clear()
    {
        data_list.Clear();
    }

    /**
    * 清除所有数据，但不释放内存
    */
    template<typename T>
    void Set<T>::ClearData()
    {
        data_list.ClearData();
    }

    /**
     * 随机取得一个数据
     */
    template<typename T>
    bool Set<T>::Rand(T &result)const
    {
        return data_list.Rand(result);
    }

    /**
     * 求当前合集与另一个数据集的交集
     * @param result 结果存放合集
     * @param list 要计算交集的数据集
     * @return 交集数量
     */
    template<typename T>
    int Set<T>::Intersection(Set<T> &result,const Set<T> &list)
    {
        if(data_list.GetCount()<=0)
            return(0);

        if(list.GetCount()<=0)
            return(0);

        data_list.Enum([&](T &obj)
        {
            if(list->IsMember(obj))
                result.Add(obj);
        });

        return result.GetCount();
    }

    template<typename T>
    int Set<T>::Intersection(const Set<T> &list)
    {
        if(data_list.GetCount()<=0)
            return(0);

        if(list.GetCount()<=0)
            return(0);

        int count=0;

        T *obj=data_list.GetData();
        for(int i=0;i<data_list.GetCount();i++)
        {
            if(list.IsMember(*obj))
                ++count;

            ++obj;
        }

        return count;
    }

    template<typename T>
    int Set<T>::Intersection(Set<T> &result,const Set<T> &il,const Set<T> &cl)
    {
        if(data_list.GetCount()<=0)
            return(0);

        if(il.GetCount()<=0)
            return(0);

        T *obj=data_list.GetData();
        for(int i=0;i<data_list.GetCount();i++)
        {
            if(il.IsMember(*obj))
                if(!cl.IsMember(*obj))
                    result.Add(*obj);
            ++obj;
        }

        return result.GetCount();
    }

    template<typename T>
    int Set<T>::Difference(const Set<T> &is)
    {
        if(data_list.GetCount()<=0)
            return(is.GetCount());

        if(is.GetCount()<=0)
            return(data_list.GetCount());

        int count=0;

        T *obj=data_list.GetData();
        for(int i=0;i<data_list.GetCount();i++)
        {
            if(!is.IsMember(*obj))
                ++count;

            ++obj;
        }

        return count;
    }
}//namespace hgl
#endif//HGL_TYPE_SET_CPP
