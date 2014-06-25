#ifndef HGL_RING_LIST_CPP
#define HGL_RING_LIST_CPP

#include<hgl/type/RingList.h>
#include<malloc.h>
namespace hgl
{
    template<typename T>
	RingList<T>::RingList(int m)
	{
		hglSetProperty(MaxCount,this,RingList<T>::GetMax,RingList<T>::SetMax);

		max_count=m;
        rpos=0;
        wpos=0;

        if(max_count)
			item=(T *)hgl_malloc(max_count*sizeof(T));
        else item=nullptr;
    }

    template<typename T>
    RingList<T>::~RingList()
    {
		if(max_count)
        {
            hgl_free(item);
            item=nullptr;
            rpos=0;
            wpos=0;
        }
    }

    template<typename T>
    void RingList<T>::SetMax(int n)
    {
		if(max_count)
        {
			if(n==max_count)return;

            if(n)
            {
				max_count=n;
				if(rpos>=max_count)rpos=0;
				if(wpos>=max_count)wpos=0;

				item=(T *)hgl_realloc(item,max_count*sizeof(T));
            }
            else Clear();
        }
        else if(n>0)
        {
			max_count=n;
            rpos=0;
            wpos=0;

			item=(T *)hgl_malloc(max_count*sizeof(T));
        }
    }

    template<typename T>
    void RingList<T>::Clear()
    {
        if(item)
        {
			max_count=0;
            rpos=0;
            wpos=0;

            hgl_free(item);
            item=nullptr;
        }
    }

    template<typename T>
    void RingList<T>::SetItem(int n,T &t)
    {
		if(n>=max_count)return;

		item[n]=t;
    }

    template<typename T>
    T &RingList<T>::GetItem(int n)
    {
		if(n>=max_count)return(*(T *)NULL);

        return(item[n]);
    }

    template<typename T>
    bool RingList<T>::Push(const T &t)
    {
		if(max_count==0)return(false);

		item[wpos++]=t;

		if(wpos>=max_count)wpos=0;

        return(true);
    }

    template<typename T>
    bool RingList<T>::Pop(T &t)
    {
		if(max_count==0)return(false);

		t=item[rpos++];

		if(rpos>=max_count)rpos=0;

        return(true);
    }

    template<typename T>
    bool RingList<T>::Peek(T &t)
    {
		if(max_count==0)return(false);

		t=item[rpos];

		return(true);
    }
}//namespace hgl
#endif//HGL_RING_LIST_CPP
