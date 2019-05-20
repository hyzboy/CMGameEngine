#ifndef HGL_TYPE_LRU_CACHE_CPP
#define HGL_TYPE_LRU_CACHE_CPP

#include<hgl/LogInfo.h>
namespace hgl
{
    /**
    * 本类构造函数
    * @param value 缓冲区最大数据量
    */
    template<typename F,typename T>
    LRUCache<F,T>::LRUCache(int value)
    {
        if(value<=0)
        {
            LOG_ERROR(OS_TEXT("ActiveChain缓冲区大小被设置<=0"));

            value=3;
        }

        count=0;
        max_count=value;

        start_item=nullptr;
        end_item=nullptr;
    }

    template<typename F,typename T>
    LRUCache<F,T>::~LRUCache()
    {
        Clear();
    }

    template<typename F,typename T>
    void LRUCache<F,T>::SetMaxCount(int value)
    {
        if(value<=0)
        {
            LOG_ERROR(OS_TEXT("ActiveChain缓冲区大小被设置<=0，此次设置无效！"));
        }
        else
            max_count=value;
    }

    template<typename F,typename T>
    bool LRUCache<F,T>::Create(const F &,T &)
    {
        return true;
    }

    template<typename F,typename T>
    void LRUCache<F,T>::Clear(const F &,T &)
    {
    }

    template<typename F,typename T>
    void LRUCache<F,T>::ClearEnd()
    {
        LruItem *temp=end_item->prev;

        Clear(end_item->key,end_item->value);

        delete end_item;

        end_item=temp;

        if(end_item)                    //如果只有一个数据，end_item会为NULL
            end_item->next=nullptr;
        else
        {
            #ifdef _DEBUG
            if(count!=1)
            {
                LOG_ERROR(OS_TEXT("ActiveChain出错，end_item=nullptr,count!=1"));
            }
            #endif//
            start_item=nullptr;         //如果end_item为空，start_item也应该为空
        }

        count--;
    }

    /**
    * 添加一个数据
    * @param key 数据标识
    * @param value 数据
    */
    template<typename F,typename T>
    LRUCacheItem<F,T> *LRUCache<F,T>::Add(const F &key,const T &value)
    {
        LruItem *temp;

        while(count>=max_count)ClearEnd();          //满了，清除超出的数据

        temp=new LruItem;
        temp->key=key;
        temp->value=value;

        temp->prev=nullptr;
        temp->next=start_item;

        if(start_item)                  //如果还没有数据,start会为NULL
            start_item->prev=temp;

        start_item=temp;                //将当前数据设成start_item

        count++;

        if(!end_item)
        {
            #ifdef _DEBUG               //理由上end_item为NULL时应该是没有数据
            if(count!=1)
            {
                LOG_ERROR(OS_TEXT("ActiveChain出错，end_item=nullptr,count!=1"));
            }
            else
            #endif//_DEBUG
                end_item=start_item;
        }

        return(temp);
    }

    template<typename F,typename T>
    void LRUCache<F,T>::MoveToStart(LruItem *item)
    {
        if(item==start_item)
            return;
                                                    //不是首节点,这个情况下count肯定>1
        if(item!=end_item)                          //也不是尾节点,这个情况下count肯定>2
        {
            if(item->next)
                item->next->prev=item->prev;

            if(item->prev)
                item->prev->next=item->next;

            start_item->prev=item;

            item->prev=nullptr;
            item->next=start_item;

            start_item=item;
        }
        else        //为尾节点
        {
            end_item=end_item->prev;
            end_item->next=nullptr;

            item->prev=nullptr;
            item->next=start_item;

            start_item->prev=item;

            start_item=item;
        }
    }

    /**
    * 取得一个数据,在没有数据时,不自动创建数据,返回false
    * @param key 数据标识
    * @param value 数据存放地
    * @param mts 是否对数据调频
    * @return 是否取得数据成功
    */
    template<typename F,typename T>
    bool LRUCache<F,T>::Find(const F &key,T &value,bool mts)
    {
        if(count<=0)return(false);

        int n=count;
        LruItem *temp=start_item;

        while(n--)
        {
            if(temp->key==key)
            {
                value=temp->value;

                if(mts)
                    MoveToStart(temp);

                return(true);
            }
            else
                temp=temp->next;
        }

        return(false);
    }

    /**
    * 取得一个数据,如果数据不存在,调用Create创建数据,如失败返回false
    * @param key 数据标识
    * @param value 数据存放地
    * @param mts 是否对数据动态调频
    * @return 是否取得数据 true/false
    */
    template<typename F,typename T>
    bool LRUCache<F,T>::Get(const F &key,T &value,bool mts)
    {
        if(Find(key,value,mts))
            return(true);

        while(count>=max_count)ClearEnd();          //满了，清除超出的数据

        if(Create(key,value))
        {
            Add(key,value);

            return(true);
        }

        return(false);
    }

    /**
    * 清数所有数据
    */
    template<typename F,typename T>
    void LRUCache<F,T>::Clear()
    {
        if(count<=0)return;

        int n=0;
        LruItem *temp=start_item;

        while(temp)
        {
            LruItem *obj=temp;

            Clear(obj->key,obj->value);

            temp=obj->next;

            delete obj;
            n++;
        }

        if(n!=count)
        {
            LOG_ERROR(OS_TEXT("LRUCache Count=")+OSString(count)+OS_TEXT(",Clear=")+OSString(n));
        }

        count=0;
        start_item=nullptr;
        end_item=nullptr;
    }

    template<typename F,typename T>
    void LRUCache<F,T>::Delete(LruItem *obj)
    {
        if(!obj)return;

        Clear(obj->key,obj->value);

        if(count>1)
        {
            if(obj==start_item)
            {
                start_item=obj->next;

                start_item->prev=nullptr;
            }
            else
            if(obj==end_item)
            {
                end_item=obj->prev;

                end_item->next=nullptr;
            }
            else
            {
                obj->prev->next=obj->next;

                obj->next->prev=obj->prev;
            }
        }
        else
        {
            start_item=nullptr;
            end_item=nullptr;
        }

        delete obj;

        count--;
    }

    template<typename F,typename T>
    void LRUCache<F,T>::DeleteByKey(const F &key)
    {
        if(count<=0)return;

        int n=count;
        LruItem *temp=start_item;

        while(n--)
        {
            if(temp->key==key)
            {
                Delete(temp);
                return;
            }

            temp=temp->next;
        }
    }

    template<typename F,typename T>
    void LRUCache<F,T>::DeleteByValue(T &value)
    {
        if(count<=0)return;

        int n=count;
        LruItem *temp=start_item;

        while(n--)
        {
            if(temp->value==value)
            {
                Delete(temp);
                return;
            }

            temp=temp->next;
        }
    }

//  template<typename F,typename T>
//  bool LRUCache<F,T>::Update(const F &key,T &value)
//  {
//      if(Find(key,value,false))
//      {
//          LruItem *obj=temp;
//
//          Clear(obj->key,obj->value);
//
//          Create(obj->key,obj->value);
//
//          return(true);
//      }
//
//      if(Create(key,value))
//      {
//          Add(key,value);
//
//          return(true);
//      }
//
//      return(false);
//  }
}//namespace hgl
#endif//HGL_TYPE_LRU_CACHE_CPP
