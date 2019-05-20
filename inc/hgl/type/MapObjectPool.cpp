#ifndef HGL_MAP_OBJECT_POOL_CPP
#define HGL_MAP_OBJECT_POOL_CPP

#include<hgl/type/Pool.h>
#include<hgl/Other.h>
namespace hgl
{
	template<typename I,typename T>
	MapObjectPool<I,T>::MapObjectPool(T **all_list,int count)
	{
		null_list.Push(all_list,count);															//将对象全部压入堆栈
	}

	template<typename I,typename T>
	MapObjectPool<I,T>::~MapObjectPool()
	{
		null_list.Clear();
		idle_list.Clear();
		active_list.Clear();
	}

	template<typename I,typename T>
	T *MapObjectPool<I,T>::Acquire()
	{
		if(null_list.Count==0)																	//未用对象列表里没有空的可以用
		{
			if(idle_list.Count==0)																//闲置对象列表里没有空的可以用
			{
				return(nullptr);																	//返回申请失败
			}

			//算法上，理应从idle_list中找出最早放入队列中的数据，但此时尚未有解决方案，所以暂使用第0个
			T *obj;
			I flag;

			if(idle_list.Get(0,flag,obj))
			{
				idle_list.DeleteBySerial(0);

				active_list.Add(obj);																//添加到活动对象列表

				return(obj);
			}
		}
		else
		{
			T *obj;

			if(null_list.Pop(obj))																	//弹出一个对象
			{
				active_list.Add(obj);																//添加到活动对象列表

				return(obj);																		//返回对象
			}
		}

		return(nullptr);
	}

	template<typename I,typename T>
	T *MapObjectPool<I,T>::TryAcquire(const I &flag)
	{
		T *obj;

		if(!idle_list.Get(flag,obj))															//尝试在闲置列表里取得这个对象
			return(nullptr);

		idle_list.DeleteByKey(flag);															//从闲置列表中清除这个数据

		active_list.Add(obj);																	//将数据添加到活动列表中

		return(obj);
	}

	template<typename I,typename T>
	T *MapObjectPool<I,T>::Acquire(const I &flag)
	{
		T *obj=TryAcquire(flag);																//尝试申请一个对象

		if(obj)return(obj);

		return Acquire();
	}

	template<typename I,typename T>
	bool MapObjectPool<I,T>::Release(T *obj)
	{
		int pos=active_list.Find(obj);															//查找数据是否在活动列表中

		if(pos==-1)return(false);

		active_list.DeleteBySerial(pos);														//从活动列表中删除这个数据

		null_list.Push(obj);																	//将数据压入未用数据列表

		return(true);
	}

	template<typename I,typename T>
	bool MapObjectPool<I,T>::Release(T *obj,const I &flag)
	{
		int pos=active_list.Find(obj);															//查找数据是否在活动列表中

		if(pos==-1)return(false);

		active_list.DeleteBySerial(pos);														//从活动列表中删除这个数据

		idle_list.Add(flag,obj);																//将数据放入闲置数据列表

		return(true);
	}
}//namespace hgl
#endif//HGL_MAP_OBJECT_POOL_CPP
