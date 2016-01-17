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

			Clear(obj->second);
		}

		items.Clear();
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

			//items[index]->count++;
			obj->count++;

//			return(items[index]->data);
			return obj->second;
		}

		T *data=Create(flag);

		if(data)
		{
			items.Add(flag,data);

			return(data);
		}

		return(nullptr);
	}

	template<typename F,typename T>
	void ResManage<F,T>::ReleaseBySerial(int index)
	{
		if(index==-1)
		{
//        	ErrorHint(u"所释放的资源不存在");
			return;
		}

		ResItem *obj=items.GetItem(index);

		if(--obj->count==0)
		{
			Clear(obj->second);

			items.DeleteBySerial(index);
		}
	}

	template<typename F,typename T>
	void ResManage<F,T>::Release(const F &flag)
	{
    	ReleaseBySerial(items.Find(flag));
	}

	template<typename F,typename T>
	void ResManage<F,T>::Release(T *td)
	{
		ReleaseBySerial(items.FindByData(td));
	}
}//namespace hgl
#endif//HGL_RES_MANAGE_CPP
