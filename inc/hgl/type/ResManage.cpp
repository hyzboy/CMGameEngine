#ifndef HGL_RES_MANAGE_CPP
#define HGL_RES_MANAGE_CPP

#include<hgl/type/ResManage.h>
namespace hgl
{
    template<typename F,typename T>
    ResManage<F,T>::~ResManage()
    {
        Clear();
    }

    template<typename F,typename T>
    void ResManage<F,T>::Clear()
    {
        int n=items.GetCount();

        while(n--)
        {
            ResItem *obj=items.GetItem(n);

            Clear(obj->right);
        }

        items.Clear();
    }

    template<typename F,typename T>
    void ResManage<F,T>::ClearFree()
    {
        int n=items.GetCount();

        while(n--)
        {
            ResItem *obj=items.GetItem(n);

            if(obj->count<=0)
            {
                Clear(obj->right);
                items.DeleteBySerial(n);
            }
        }
    }

    template<typename F,typename T>
    bool ResManage<F,T>::Add(const F &flag,T *obj)
    {
        if(!obj)return(false);

        if(items.KeyExist(flag))
            return(false);

        items.Add(flag,obj);
        return(true);
    }

    template<typename F,typename T>
    T *ResManage<F,T>::Find(const F &flag)
    {
        int index=items.Find(flag);

        if(index==-1)
            return(nullptr);

        T *result;

        if(items.Get(flag,result))
            return(result);

        return(nullptr);
    }

    template<typename F,typename T>
    T *ResManage<F,T>::Get(const F &flag)
    {
        int index=items.Find(flag);

        if(index!=-1)
        {
            ResItem *obj=items.GetItem(index);

            ++obj->count;

            return obj->right;
        }

        return(nullptr);
    }

    template<typename F,typename T>
    void ResManage<F,T>::ReleaseBySerial(int index,bool zero_clear)
    {
        if(index==-1)
        {
//          ErrorHint(u"所释放的资源不存在");
            return;
        }

        ResItem *obj=items.GetItem(index);

        --obj->count;

        if(zero_clear&&obj->count==0)
        {
            Clear(obj->right);

            items.DeleteBySerial(index);
        }
    }

    template<typename F,typename T>
    void ResManage<F,T>::Release(const F &flag,bool zero_clear)
    {
        ReleaseBySerial(items.Find(flag),zero_clear);
    }

    template<typename F,typename T>
    void ResManage<F,T>::Release(T *td,bool zero_clear)
    {
        ReleaseBySerial(items.FindByValue(td),zero_clear);
    }
}//namespace hgl
#endif//HGL_RES_MANAGE_CPP
