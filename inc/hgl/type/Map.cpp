#ifndef HGL_MAP_CPP
#define HGL_MAP_CPP

namespace hgl
{
    /**
    * 查找数据是否存在
    * @param flag 数据标识
    * @return 数据所在索引，-1表示不存在
    */
    template<typename F,typename T,typename DataPair>
    int _Map<F,T,DataPair>::Find(const F &flag)const
    {
        int left=0,right=data_list.GetCount()-1;                //使用left,right而不使用min,max是为了让代码能够更好的阅读。
        int mid;

        DataPair **data_array=data_list.GetData();

        while(left<=right)
        {
            if(data_array[left ]->left==flag)return(left);
            if(data_array[right]->left==flag)return(right);

            mid=(right+left)>>1;

            if(data_array[mid]->left==flag)return(mid);

            if(data_array[mid]->left>flag)
            {
                ++left;
                right=mid-1;
            }
            else
            {
                --right;
                left=mid+1;
            }
        }

//      LOG_PROBLEM(OS_TEXT("Map::Find,no result."));
        return(-1);
    }

    template<typename F,typename T,typename DataPair>
    bool _Map<F,T,DataPair>::FindPos(const F &flag,int &pos)const
    {
        int left=0,right=data_list.GetCount()-1;
        int mid;

        DataPair **data_array=data_list.GetData();

        while(left<=right)
        {
            if(data_array[left ]->left>flag)
            {
                pos=left;
                return(false);
            }
            else
            if(data_array[left ]->left==flag)
            {
                pos=left;
                return(true);
            }

            if(data_array[right]->left<flag)
            {
                pos=right+1;
                return(false);
            }
            else
            if(data_array[right]->left==flag)
            {
                pos=right;
                return(true);
            }

            mid=(right+left)>>1;

            if(data_array[mid]->left==flag)
            {
                pos=mid;
                return(true);
            }

            if(data_array[mid]->left>flag)
            {
                if(data_array[mid-1]->left<flag)
                {
                    pos=mid;
                    return(false);
                }
                else
                if(data_array[mid-1]->left==flag)
                {
                    pos=mid-1;
                    return(true);
                }

                ++left;
                right=mid-1;
            }
            else
            {
                if(data_array[mid+1]->left>flag)
                {
                    pos=mid+1;
                    return(false);
                }
                else
                if(data_array[mid+1]->left==flag)
                {
                    pos=mid+1;
                    return(true);
                }

                --right;
                left=mid+1;
            }
        }

//      LOG_PROBLEM(OS_TEXT("Map::FindPos,no result."));
        pos=-1;
        return(false);
    }

    template<typename F,typename T,typename DataPair>
    int _Map<F,T,DataPair>::FindByValue(const T &data)const
    {
        const int count=data_list.GetCount();

        DataPair **data_array=data_list.GetData();

        for(int i=0;i<count;i++)
        {
            if((*data_array)->right==data)
                return(i);

            ++data_array;
        }

        return -1;
    }

    /**
    * 添加一个数据
    * @param flag 数据标识
    * @param data 数据
    * @return 新创建好的数据结构
    */
    template<typename F,typename T,typename DataPair>
    DataPair *_Map<F,T,DataPair>::Add(const F &flag,const T &data)
    {
        DataPair *ds=data_pool.Acquire();

        ds->left=flag;
        ds->right=data;

        int pos;

        if(FindPos(flag,pos))
            return(nullptr);

        data_list.Insert(pos,ds);

        return(ds);
    }

    /**
    * 添加一个数据
    * @param flag 数据标识
    * @return 创建好的数据
    */
    template<typename F,typename T,typename DataPair>
    T &_Map<F,T,DataPair>::Add(const F &flag)
    {
        DataPair *ds=data_pool.Acquire();

        ds->left=flag;

        int pos;

        FindPos(flag,pos);

        data_list.Insert(pos,ds);

        return ds->right;
    }

    /**
    * 添加一个数据
    * @param obj 数据
    */
    template<typename F,typename T,typename DataPair>
    void _Map<F,T,DataPair>::Add(DataPair *obj)
    {
        data_list.Insert(FindPos(obj->left),obj);
    }

    /**
    * 根据索引取得数据
    * @param flag 数据索引
    * @param data 数据存放处
    * @return 是否取得成功
    */
    template<typename F,typename T,typename DataPair>
    bool _Map<F,T,DataPair>::Get(const F &flag,T &data) const
    {
        int index=Find(flag);

        DataPair *obj=GetListObject(data_list,index);

        if(!obj)
            return(false);

        data=obj->right;

        return(true);
    }

    template<typename F,typename T,typename DataPair>
    bool _Map<F,T,DataPair>::Check(const F &key,const T &value) const
    {
        int index=Find(key);

        DataPair *obj=GetListObject(data_list,index);

        if(!obj)
            return(false);

        return (value==obj->right);
    }

    /**
    * 根据序号取得数据
    * @param index 序号
    * @param f 数据索引存放处
    * @param t 数据存放处
    * @return 是否取得成功
    */
    template<typename F,typename T,typename DataPair>
    bool _Map<F,T,DataPair>::Get(int index,F &f,T &t) const
    {
        if(index<0||index>=data_list.GetCount())return(false);

        DataPair *ds=data_list[index];

        f=ds->left;
        t=ds->right;

        return(true);
    }

    /**
    * 根据序号取得索引
    * @param index 序号
    * @param f 数据索引存放处
    * @return 是否取得成功
    */
    template<typename F,typename T,typename DataPair>
    bool _Map<F,T,DataPair>::GetKey(int index,F &f)
    {
        if(index<0||index>=data_list.GetCount())return(false);

        DataPair *ds=data_list[index];

        f=ds->left;

        return(true);
    }

    /**
    * 根据序号取得数据
    * @param index 序号
    * @param t 数据存放处
    * @return 是否取得成功
    */
    template<typename F,typename T,typename DataPair>
    bool _Map<F,T,DataPair>::GetValue(int index,T &t)
    {
        if(index<0||index>=data_list.GetCount())return(false);

        DataPair *ds=data_list[index];

        if(!ds)return(false);

        t=ds->right;

        return(true);
    }

    /**
     * 根据序号设置数据
     * @param index 数据序号
     * @param t 数据
     */
    template<typename F,typename T,typename DataPair>
    bool _Map<F,T,DataPair>::SetValueBySerial(int index,T &t)
    {
        if(index<0||index>=data_list.GetCount())return(false);

        data_list[index]->right=t;

        return(true);
    }

    /**
    * 将指定数据从列表中移除同时取得这个数据
    * @param flag 数据标识
    * @param data 数据存放位处
    * @return 是否成功
    */
    template<typename F,typename T,typename DataPair>
    bool _Map<F,T,DataPair>::Delete(const F &flag,T &data)
    {
        int index=Find(flag);

        DataPair *dp=GetListObject(data_list,index);

        if(!dp)
            return(false);

        data=dp->right;

        data_pool.Release(dp);
        data_list.DeleteMove(index);

        return(true);
    }

    /**
    * 根据索引将指定数据从列表中移除
    * @param flag 索引
    * @return 是否成功
    */
    template<typename F,typename T,typename DataPair>
    bool _Map<F,T,DataPair>::DeleteByKey(const F &flag)
    {
        return DeleteBySerial(Find(flag));
    }

    /**
    * 根据索引将指定数据从列表中批量移除
    * @param fp 索引列表
    * @param count 索引数量
    * @return 是否成功
    */
    template<typename F,typename T,typename DataPair>
    int _Map<F,T,DataPair>::DeleteByKey(const F *fp,const int count)
    {
        if(!fp||count<=0)return(0);

        int total=0;

        for(int i=0;i<count;i++)
        {
            if(DeleteBySerial(Find(*fp)))
                ++total;

            ++fp;
        }

        return(total);
    }


    /**
    * 根据数据将指定数据从列表中移除
    * @param data 数据
    * @return 是否成功
    */
    template<typename F,typename T,typename DataPair>
    bool _Map<F,T,DataPair>::DeleteByValue(const T &data)
    {
        return DeleteBySerial(FindByValue(data));
    }

    /**
    * 根据序号将指定数据从列表中移除
    * @param index 序号
    * @return 是否成功
    */
    template<typename F,typename T,typename DataPair>
    bool _Map<F,T,DataPair>::DeleteBySerial(int index)
    {
        if(index<0
         ||index>=data_list.GetCount())return(false);

        data_pool.Release(GetListObject(data_list,index));
        data_list.DeleteMove(index);

        return(true);
    }

    /**
     * 根据序号将指定数据从列表中移除
     * @param start 起始序号
     * @param number 数量
     * @return 是否成功
     */
    template<typename F,typename T,typename DataPair>
    bool _Map<F,T,DataPair>::DeleteBySerial(int start,int number)
    {
        DataPair **dp=data_list.GetData()+start;

        for(int i=0;i<number;i++)
        {
            data_pool.Release(*dp);
            ++dp;
        }

        return data_list.Delete(start,number);
    }

    /**
     * 更新一个数据，如果这个数据不存在，就添加一个新的
     * @param flag 数据标识
     * @param data 新的数据内容
     */
    template<typename F,typename T,typename DataPair>
    void _Map<F,T,DataPair>::Update(const F &flag,const T &data)
    {
        int result;

        if(FindPos(flag,result))
        {
            DataPair *dp=GetListObject(data_list,result);

            if(dp)
                dp->right=data;
        }
        else
        {
            DataPair *ds=data_pool.Acquire();

            ds->left=flag;
            ds->right=data;

            data_list.Insert(result,ds);
        }
    }

    /**
    * 更改数据,这个更改和Set不同,它要求指定标识的数据必须存在，则否就会更改失败
    * @param flag 数据标识
    * @param data 新的数据内容
    * @param return 是否更改成功
    */
    template<typename F,typename T,typename DataPair>
    bool _Map<F,T,DataPair>::Change(const F &flag,const T &data)
    {
        DataPair *dp=GetListObject(data_list,Find(flag));

        if(!dp)
            return(false);

        dp->right=data;
        return(true);
    }

    /**
    * 清除所有数据
    */
    template<typename F,typename T,typename DataPair>
    void _Map<F,T,DataPair>::Clear()
    {
        data_pool.ClearAll();
        data_list.Clear();
    }

    /**
    * 清除所有数据，但不释放内存
    */
    template<typename F,typename T,typename DataPair>
    void _Map<F,T,DataPair>::ClearData()
    {
        data_pool.ReleaseAll();
        data_list.ClearData();
    }

    template<typename F,typename T,typename DataPair>
    void _Map<F,T,DataPair>::operator=(const _Map<F,T,DataPair> &ftd)
    {
        Clear();

        data_pool.ClearAll();
        data_list.ClearData();

        const int count=ftd.data_list.GetCount();

        if(count<=0)
            return;

        IDItem **obj=ftd.data_list.GetData();

        for(int i=0;i<count;i++)
        {
            IDItem *new_obj=data_pool.Acquire();

            new_obj->left=(*obj)->left;
            new_obj->right=(*obj)->right;

            data_list.Add(new_obj);

            ++obj;
        }
    }

    template<typename F,typename T,typename DataPair>
    void _Map<F,T,DataPair>::Enum(void (*enum_func)(const F &,T))
    {
        const int count=data_list.GetCount();

        if(count<=0)
            return;

        IDItem **idp=data_list.GetData();

        for(int i=0;i<count;i++)
        {
            enum_func((*idp)->left,(*idp)->right);

            ++idp;
        }
    }

    template<typename F,typename T,typename DataPair>
    void _Map<F,T,DataPair>::EnumKey(void (*enum_func)(const F &))
    {
        const int count=data_list.GetCount();

        if(count<=0)
            return;

        IDItem **idp=data_list.GetData();

        for(int i=0;i<count;i++)
        {
            enum_func((*idp)->left);

            ++idp;
        }
    }

    template<typename F,typename T,typename DataPair>
    void _Map<F,T,DataPair>::EnumAllValue(void (*enum_func)(T))
    {
        const int count=data_list.GetCount();

        if(count<=0)
            return;

        IDItem **idp=data_list.GetData();

        for(int i=0;i<count;i++)
        {
            enum_func((*idp)->right);

            ++idp;
        }
    }

    template<typename F,typename T,typename DataPair>
    void _Map<F,T,DataPair>::EnumValue(bool (*enum_func)(T))
    {
        const int count=data_list.GetCount();

        if(count<=0)
            return;

        IDItem **idp=data_list.GetData();

        for(int i=0;i<count;i++)
        {
            if(!enum_func((*idp)->right))
                return;

            ++idp;
        }
    }
}//namespace hgl
#endif//HGL_MAP_CPP
