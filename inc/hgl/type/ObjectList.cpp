#ifndef HGL_OBJECT_LIST_CPP
#define HGL_OBJECT_LIST_CPP

#include<hgl/type/List.h>
namespace hgl
{
	/**
	* 对象列表析构函数，会调用DeleteAll函数
	*/
	template<typename T>
	CusObjectList<T>::~CusObjectList()
	{
		Clear();
	}

//	/**
//	* 生成一个对象，并返回它的指针
//	*/
//	template<typename T>
//	T *CusObjectList<T>::Append()
//	{
//		if(!this->items)
//		{
//			this->max_count=1;
//			this->items=(T **)hgl_aligned_malloc<T *>(1);
//		}
//		else
//		{
//			if(this->count>=this->max_count)
//				this->max_count<<=1;
//
//			this->items=(T **)hgl_realloc(this->items,this->max_count*sizeof(T *));
//		}
//
//		return(this->items[this->count++]=CreateObject());
//	}

//	/**
//	* 在指定位置插入一个对象
//	*/
//	template<typename T>
//	T *CusObjectList<T>::Insert(int index)
//	{
//		if(index<0)index=0;
//
//		if(index<this->count)
//		{
//			if(this->count>=this->max_count)
//				this->max_count<<=1;
//
//			this->items=(T **)hgl_realloc(this->items,this->max_count*sizeof(T *));
//
//			memmove(this->items+index+1,this->items+index,(this->count-index)*sizeof(T *));
//
//			this->count++;
//
//			return(this->items[index]=CreateObject());
//		}
//
//		return(Append());
//	}

	/**
	* 在指定位置插入一个对象
	*/
	template<typename T>
	void CusObjectList<T>::Insert(int index,const ItemPointer &obj)
	{
		List<T *>::Insert(index,obj);
	}

	/**
	* 清除所有对象,作用和DeleteAll一样
	*/
	template<typename T>
	void CusObjectList<T>::Clear()
	{
		DeleteAll();
		List<T *>::Clear();
	}

	/**
	* 清除所有对象,但不释放内存
	*/
	template<typename T>
	void CusObjectList<T>::ClearData()
	{
		DeleteAll();
		List<T *>::ClearData();
	}

	/**
	* 删除列表中的指定项
	*
	* 这个函数在删除指定对象时，附加使用delete方法
	* @param index 要删除的对象的索引值
	* @return 是否成功
	*/
	template<typename T>
	bool CusObjectList<T>::Delete(int index)
	{
		if(index>=0&&index<this->count)
		{
			DeleteObject(this->items[index]);

			this->count--;

			if(index<this->count)
				memcpy(this->items+index,this->items+this->count,sizeof(T *));

			return(true);
		}
		else
        	return(false);
	}

	/**
	* 删除列表中的指定项
	*
	* 这个函数在删除指定对象时，附加使用delete方法
	* @param index 要删除的对象的索引值
	* @return 是否成功
	*/
	template<typename T>
	bool CusObjectList<T>::DeleteMove(int index)
	{
		if(index>=0&&index<this->count)
		{
			DeleteObject(this->items[index]);

			this->count--;

			if(index<this->count)
				memmove(this->items+index,this->items+index+1,(this->count-index)*sizeof(T *));

			return(true);
		}
		else
        	return(false);
	}

	/**
	* 删除列表中的指定项
	*
	* 这个函数在删除指定对象时，附加使用delete方法
	* @param obj 要删除的对象
	* @return 是否成功
	*/
	template<typename T>
	bool CusObjectList<T>::DeleteByValue(const ItemPointer &obj)
	{
		int n=this->count;

		while(n--)
		{
			if(this->items[n]==obj)
			{
				DeleteObject(this->items[n]);

				this->count--;

				if(n<this->count)
					memmove(this->items+n,this->items+n+1,(this->count-n)*sizeof(T *));

				return(true);
			}
		}

		return(false);
	}

	/**
	* 将一批对象从列表中删除
	* @param obj 要删除的对象
	* @param n 要删除的对象个数
	*/
	template<typename T>
	void CusObjectList<T>::DeleteByValue(const ItemPointer *obj,int n)
	{
		while(n--)
		{
			int index=List<T *>::Find(*obj);

			obj++;

			if(index!=-1)
				Delete(index);
		}
	}
	/**
	* 删除整个列表中的所有对象
	*
	* 这个函数在删除每一个对象时，都会使用一次delete
	*/
	template<typename T>
	void CusObjectList<T>::DeleteAll()
	{
		if(this->count)
		{
			int n=this->count;

			while(n--)
				DeleteObject(this->items[n]);

			this->count=0;
		}
	}

    template<typename T>
    void CusObjectList<T>::SetCount(int new_count)
    {
		if(this->count==new_count)return;

        if(new_count<=0)
        {
            DeleteAll();
        }
        else
        {
			this->max_count=power_to_2(new_count);

			if(this->items)
            {
				if(new_count>this->count)
                {
					this->items=(T **)hgl_realloc(this->items,this->max_count*sizeof(T *));

//					for(;this->count<new_count;this->count++)
//						this->items[this->count]=CreateObject();
                }
                else
                {
					while(this->count-->new_count)
						DeleteObject(this->items[this->count]);

					this->items=(T **)hgl_realloc(this->items,this->max_count*sizeof(T *));
                }
            }
//            else
//            {
//				this->items=(T **)hgl_aligned_malloc<T *>(this->max_count);
//
//                while(new_count--)
//					this->items[this->count++]=CreateObject();
//            }
        }
	}
}//namespace hgl
#endif//HGL_OBJECT_LIST_CPP
