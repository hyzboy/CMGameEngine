#ifndef HGL_POOL_CPP
#define HGL_POOL_CPP

#include<hgl/type/DataType.h>
#include<hgl/type/Pool.h>
namespace hgl
{
	template<typename T>
	void Pool<T>::PreMalloc(int num)
	{
		if(num<=0)return;

		for(int i=0;i<num;i++)
			Inactive.Push(Create());

		count+=num;
		if(count>history_max)
			history_max=count;
	}

	template<typename T>
	T Pool<T>::Acquire()
	{
		T result;

		if(!Inactive.Pop(result))
		{
			result=Create();

			count++;

			if(count>history_max)
				history_max=count;
		}

		Active.Add(result);

		return(result);
	}

	template<typename T>
	void Pool<T>::Append(T value)
	{
		T result;

		if(!Inactive.Pop(result))
		{
			count++;

			if(count>history_max)
				history_max=count;
		}

		Active.Add(result);

		result=value;
	}

	template<typename T>
	bool Pool<T>::Release(T value)
	{
		int index=Active.Find(value);

		if(index!=-1)
		{
			Active.Delete(index);

			Inactive.Push(value);

			return(true);
		}

		return(false);
	}

	template<typename T>
	int Pool<T>::Release(T *vl,int count)
	{
		int total=0;

		for(int i=0;i<count;i++)
		{
			if(Release(*vl))
				++total;

			++vl;
		}

		return(total);
	}

	template<typename T>
	int Pool<T>::ReleaseAll()
	{
		int count=Active.GetCount();
		T *p=Active.GetData();

		for(int i=0;i<count;i++)
			Inactive.Push(*p++);

		Active.ClearData();
		return(count);
	}

	template<typename T>
	void Pool<T>::ClearInactive()
	{
		T result;

		count-=Inactive.GetCount();

		while(Inactive.Pop(result))
			Clear(result);
	}

	template<typename T>
	void Pool<T>::ClearAll()
	{
		{					//不能直接调ClearInactive
			T result;

			count-=Inactive.GetCount();

			while(Inactive.Pop(result))
				Clear(result);
		}

		{
			int n=Active.GetCount();
            T *p=Active.GetData();

			while(n--)
            {
				Clear(*p);
                ++p;
            }

			Active.Clear();
		}

		count=0;
	}
}//namespace hgl
#endif//HGL_POOL_CPP
