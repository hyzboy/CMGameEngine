#ifndef HGL_CLASLIST_CPP
#define HGL_CLASLIST_CPP

#include<hgl/type/ClasList.h>
namespace hgl
{
    template<typename T,int TypeCount>
    void ClasList<T,TypeCount>::Clear()
    {
        for(int i=0;i<TypeCount;i++)
            clas_object[i].Clear();

        all_object.Clear();
    }

    template<typename T,int TypeCount>
    bool ClasList<T,TypeCount>::Unlink(T *obj)
    {
        if(!obj)return(false);

        for(int i=0;i<TypeCount;i++)
            clas_object[i].DeleteByValue(obj);

        return all_object.UnlinkByValue(obj);
    }

    template<typename T,int TypeCount>
    void ClasList<T,TypeCount>::UnlinkAll()
    {
        for(int i=0;i<TypeCount;i++)
            clas_object[i].Clear();

        all_object.UnlinkAll();
    }

    template<typename T,int TypeCount>
    bool ClasList<T,TypeCount>::Delete(T *obj)
    {
        if(!obj)return(false);

        for(int i=0;i<TypeCount;i++)
            clas_object[i].DeleteByValue(obj);

        return all_object.DeleteByValue(obj);
    }

    template<typename T,int TypeCount>
    uint ClasList<T,TypeCount>::Add(T *obj)
    {
        if(!obj)return(0);

        int count=0;

        for(int i=0;i<TypeCount;i++)
            if(CheckObjectAttrib(obj,i))
            {
                clas_object[i].Add(obj);
                ++count;
            }

        if(count<=0)return(0);

        all_object+=obj;
        return(count);
    }
}//namespace hgl
#endif//HGL_CLASLIST_CPP
