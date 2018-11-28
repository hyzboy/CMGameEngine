#ifndef HGL_LIST_CPP
#define HGL_LIST_CPP

#include<string.h>
#include<hgl/LogInfo.h>
//--------------------------------------------------------------------------------------------------
// 代码中的部分memcpy可替换为memmove,但这样会引起CodeGuard/Valgrind之类的内存调试器报错
//--------------------------------------------------------------------------------------------------
namespace hgl
{
	template<typename T>
	bool List<T>::Get(int index,T &ti)const
	{
		if(!items||index<0||index>=count)
		{
            LOG_ERROR(OS_TEXT("List<>::Get(index=")+OSString(index)+OS_TEXT(") error,DataCount=")+OSString(count));
			return(false);
		}

		memcpy(&ti,items+index,sizeof(T));
		return(true);
	}

	template<typename T>
	bool List<T>::Begin(T &ti)const
	{
		if(!items)
			return(false);

		memcpy(&ti,items,sizeof(T));
		return(true);
	}

	template<typename T>
	bool List<T>::End(T &ti)const
	{
		if(!items)
			return(false);

		memcpy(&ti,items+count-1,sizeof(T));
		return(true);
	}

	template<typename T>
	bool List<T>::Rand(T &ti)const
	{
		if(!items||count<=0)
			return(false);

		memcpy(&ti,items+(lrand48()%count),sizeof(T));
		return(true);
	}

	template<typename T>
	void List<T>::Set(int index,const T &val)
	{
		#ifdef _DEBUG
			if(!items||index<0||index>=count)
			{
                LOG_ERROR(OS_TEXT("List<>::Set(index=")+OSString(index)+OS_TEXT(",T &) error,DataCount=")+OSString(count));
			}
			else
				memcpy(items+index,&val,sizeof(T));//items[index]=val;
		#else
			if(index>=0&&index<count)
				memcpy(items+index,&val,sizeof(T));//items[index]=val;
		#endif//_DEBUG
	}

	/**
	 * 向列表中添加一个空数据
	 * @return 这个数据的指针
	 */
	template<typename T>
	T *List<T>::Add()
	{
		if(!items)
		{
			count=1;
			max_count=1;
            items=hgl_aligned_malloc<T>(1);

			return items;
		}
		else
		{
			max_count=power_to_2(count+1);

			items=(T *)hgl_realloc(items,max_count*sizeof(T));

			++count;
			return(items+(count-1));
		}
	}

	/**
	* 向列表中添加一个数据对象
	* @param data 要添加的数据对象
	* @return 这个数据的索引号
	*/
	template<typename T>
	int List<T>::Add(const T &data)
	{
		if(!items)
		{
			count=0;
			max_count=1;
            items=hgl_aligned_malloc<T>(1);
		}
		else
		{
			max_count=power_to_2(count+1);

			items=(T *)hgl_realloc(items,max_count*sizeof(T));
		}

		memcpy(items+count,&data,sizeof(T));//items[count]=data;
		return(count++);
	}

	/**
	* 重复向列表中添加一个数据对象
	* @param data 要添加的数据对象
	* @param n 要添加的数据个数
	* @return 这个数据的索引号
	* @return -1 出错
	*/
	template<typename T>
	int List<T>::Add(const T &data,int n)
	{
		if(n<=0)return(-1);

		if(!items)
		{
			count=0;
			max_count=power_to_2(n);
            items=hgl_aligned_malloc<T>(max_count);
		}
		else
		{
			max_count=power_to_2(count+n);

			items=(T *)hgl_realloc(items,max_count*sizeof(T));
		}

		T *p=items;
		int result=count;

		for(int i=0;i<n;i++)
		{
			memcpy(p,&data,sizeof(T));//items[count]=data;
			++p;
		}

		count+=n;
		return(result);
	}

	/**
	* 向列表中添加一批数据对象
	* @param data 要添加的数据对象
	* @param n 要添加的数据数量
	* @return 起始数据的索引号
	*/
	template<typename T>
	int List<T>::Add(const T *data,int n)
	{
		if(!items)
		{
			count=0;
			max_count=power_to_2(n);

            items=hgl_aligned_malloc<T>(max_count);
		}
		else
		{
			max_count=power_to_2(count+n);

			items=(T *)hgl_realloc(items,max_count*sizeof(T));
		}

		memcpy(items+count,data,n*sizeof(T));

		int r=count;

		count+=n;
		return(r);
	}

	/**
	* 清除整个列表
	*/
	template<typename T>
	void List<T>::Clear()
	{
		count=0;
		max_count=0;

		if(items)
		{
			hgl_free(items);
			items=0;
		}
	}

	/**
	* 清除整个列表，但不清除缓冲区
	*/
	template<typename T>
	void List<T>::ClearData()
	{
		count=0;
	}

	template<typename T>
	void List<T>::DeleteClear()
	{
		if(count)
		{
			T *p=items;

			while(count--)
			{
				delete *p;
				++p;
			}

			count=0;
		}
	}

	/**
	* 在列表中查找指定的数据项
	* @param data 要查找的数据项
	* @return 数据在列表中的位置
	*/
	template<typename T>
	int List<T>::Find(const T &data)const
	{
		int n=count;

		while(n--)
//			if(items[n]==data)return(n);
			if(memcmp(items+n,&data,sizeof(T))==0)
				return(n);

		return(-1);
	}

	/**
	* 删除列表中的指定项,删除后将最后一个数据移到被删除的位置
	* @param index 要删除的数据项的索引值
	* @return 是否成功
	*/
	template<typename T>
	bool List<T>::Delete(int index)
	{
		if(count>0&&index>=0&&index<count)
		{
			--count;

			if(index<count)
				memcpy(items+index,items+count,sizeof(T));		//将最后一个数据移到当前位置

			return(true);
		}
		else
			return(false);
	}

	/**
	* 删除列表中的指定项，删除后将后面的数据整体前移
	* @param index 要删除的数据项的索引值
	* @return 是否成功
	*/
	template<typename T>
	bool List<T>::DeleteMove(int index)
	{
		if(count>0&&index>=0&&index<count)
		{
			--count;

			if(index<count)
				memmove(items+index,items+index+1,(count-index)*sizeof(T));

			return(true);
		}
		else
			return(false);
	}

	/**
	* 删除列表中的指定项
	* @param start 要删除的数据项的索引起始值
	* @param number 要删除的数据项数量
	* @return 是否成功
	*/
	template<typename T>
	bool List<T>::Delete(int start,int number)
	{
		if(start>=count)return(false);

		if(start<0)
		{
			number+=start;
			start=0;
		}

		if(start+number>count)
			number=count-start;

		if(number<=0)return(false);

		count-=number;

		if(start<count)
			memmove(items+start,items+start+number,(count-start)*sizeof(T));

		return(true);
	}

	/**
	* 删除列表中的指定项
	* @param data 要删除的数据项
	* @return 是否成功
	*/
	template<typename T>
	bool List<T>::DeleteByValue(const T &dat)
	{
		int index=Find(dat);

		if(index!=-1)
		{
			Delete(index);
			return(true);
		}
		else
			return(false);
	}

	/**
	* 删除列表中的指定项
	* @param data 要删除的数据项
	* @param n 要删除的数据个数
	*/
	template<typename T>
	void List<T>::DeleteByValue(const T *data,int n)
	{
		while(n--)
		{
			int index=Find(*data);

			++data;

			if(index!=-1)
				Delete(index);
		}
	}

	/**
	* 交换列表中两个数据的位置
	* @param a 第一个数据的位置
	* @param b 第二个数据的位置
	*/
	template<typename T>
	void List<T>::Exchange(int a,int b)
	{
		//T t;
		char t[sizeof(T)];

//		t=items[a];

//		items[a]=items[b];
//		items[b]=t;

		memcpy(&t,items+a,sizeof(T));
		memcpy(items+a,items+b,sizeof(T));
		memcpy(items+b,&t,sizeof(T));
	}

	/**
	* 在列表的指定位置中插入一个数据
	* @param index 插入数据的位置
	* @param data 要插入的数据
	*/
	template<typename T>
	void List<T>::Insert(int index,const T &data)
	{
		if(index<0)index=0;

		if(index<count)
		{
			if(!items)
			{
				max_count=1;

                items=hgl_aligned_malloc<T>(max_count);
			}
			else
			{
                max_count=power_to_2(count+1);

				items=(T *)hgl_realloc(items,max_count*sizeof(T));
			}

			memmove(items+index+1,items+index,(count-index)*sizeof(T));

			memcpy(items+index,&data,sizeof(T));//items[index]=data;

			++count;
		}
		else
			Add(data);
	}

	/**
	* 移动列表中的某一项到新的位置
	* @param index 要移动的数据位置
	* @param newindex 新的位置
	*/
	template<typename T>
	void List<T>::Move(int index,int newindex)
	{
		if(index==newindex)return;
		if(index<=0||index>=count)return;
		if(newindex<0)newindex=0;
		if(newindex>=count)newindex=count-1;

		//T t;
		char t[sizeof(T)];

		memcpy(&t,items+index,sizeof(T));//t=items[index];

		if(index<newindex)memmove(items+index      ,items+index+1  ,(newindex-index)*sizeof(T));
					else memmove(items+newindex+1 ,items+newindex ,(index-newindex)*sizeof(T));

		memcpy(items+newindex,&t,sizeof(T));//items[newindex]=t;
	}

	template<typename T>
	void List<T>::PreMalloc(int new_count)
	{
		if(max_count>=new_count)return;

		max_count=power_to_2(new_count);

		if(!items)
            items=hgl_aligned_malloc<T>(max_count);
		else
			items=(T *)hgl_realloc(items,max_count*sizeof(T));
	}

	template<typename T>
	void List<T>::SetCount(int new_count)
	{
		if(count==new_count)return;

		if(new_count<=0)
		{
			ClearData();
			return;
		}

		PreMalloc(new_count);

		count=new_count;
	}

	/**
	* 复制整个列表
	* @param lt 列表
	*/
	template<typename T>
	void List<T>::operator = (const List<T> &lt)
	{
		if(lt.count<=0)
		{
			count=0;
			return;
		}

		SetCount(lt.count);

		memcpy(items,lt.items,count*sizeof(T));
	}

	template<typename T>
	void List<T>::operator = (const std::initializer_list<T> &l)
	{
		ClearData();

		SetCount((int)l.size());

		memcpy(items,l.begin(),count*sizeof(T));
	}
}//namespace hgl
#endif//HGL_LIST_CPP
