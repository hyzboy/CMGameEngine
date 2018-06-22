#ifndef HGL_FIXEDLIST_INCLUDE
#define HGL_FIXEDLIST_INCLUDE

#include<hgl/type/DataType.h>
#include<hgl/thread/RWLock.h>
namespace hgl
{
	/**
	* 定长列表用于保存固定长度的数据
	*/
	template<typename T> class FixedList                                                            ///定长列表
	{
	protected:

		T *items;
		int max_count,count;

		void InitPrivate();

	public:	//属性

		T *GetData()const{return items;}															///<提供原始数据项

	public:	//事件

		void (*OnClear)(const T &);																	///<清除事件

	public: //方法

		FixedList();																				///<本类构造函数
		FixedList(int);                                                                             ///<本类构造函数
		virtual ~FixedList();                                                                       ///<本类析构函数

		virtual void Create(int);																	///<创建数据列表

		//为在派生类中区分安全方法与非安全方法，不可以将下面三个函数改为属性
		virtual int	 GetMaxCount()const{return max_count;}											///<取得最大数据个数
		virtual int	 GetCount()const{return count;}													///<取得当前数据个数
		virtual bool IsFull()const{return count>=max_count;}										///<是否是满的

		virtual bool Append(const T &);                                                             ///<压入一个数据
		virtual bool Append(const T *,int);															///<压入一批数据

		virtual int  Find(const T &);																///<查找数据

		virtual bool Delete(int);																	///<删除指定数据
		virtual bool Delete(const T &v){return Delete(Find(v));}									///<删除指定数据
		virtual void Delete(const T *,int);															///<删除指定数据

		virtual void Clear();                                                                       ///<清除所有数据(全部清0)

		virtual T *CreateCopy(int &);																///<创建一个拷贝

        virtual bool Get(int,T &);
	};//template<typename T> class FixedList

	template<typename T> class FixedObjectList:public FixedList<T *>                                ///定长对象列表
	{
	public:

		using FixedList<T *>::FixedList;

		virtual ~FixedObjectList(){Clear();}

		virtual bool Delete(int);
		virtual void Clear();
	};//template<typename T> class FixedObjectList

	template<typename T> class MTFixedList:public FixedList<T>										///<多线程定长数据列表
	{
		RWLock lock;

	public:

		using FixedList<T>::FixedList;
		virtual ~MTFixedList()=default;

	public:

		HGL_RWLOCK(lock);

	public:	//安全方法

		virtual void SafeCreate(int c)
		{
			lock.WriteLock();
			FixedList<T>::Create(c);
			lock.WriteUnlock();
		}

		virtual T *SafeCreateCopy(int &c)
		{
			T *copy;

			lock.ReadLock();
			copy=FixedList<T>::CreateCopy(c);
			lock.ReadUnlock();

			return copy;
		}

		int SafeGetCount()
		{
			int result;

			lock.ReadLock();
			result=FixedList<T>::count;
			lock.ReadUnlock();

			return result;
		}

		int SafeGetMaxCount()
		{
			int result;

			lock.ReadLock();
			result=FixedList<T>::max_count;
			lock.ReadUnlock();

			return result;
		}

		void SafeGetCount(int &c,int &m)
		{
			lock.ReadLock();
			c=FixedList<T>::count;
			m=FixedList<T>::max_count;
			lock.ReadUnlock();
		}

		bool SafeIsFull()
		{
			bool result;

			lock.ReadLock();
			result=FixedList<T>::IsFull();
			lock.ReadUnlock();

			return result;
		}

		virtual bool SafeAppend(const T &data)
		{
			bool result;

			lock.WriteLock();
			result=FixedList<T>::Append(data);
			lock.WriteUnlock();

			return(result);
		}

		virtual bool SafeAppend(const T *data,int n)
		{
			bool result;

			lock.WriteLock();
			result=FixedList<T>::Append(data,n);
			lock.WriteUnlock();

			return(result);
		}

		virtual int  SafeFind(const T &data)
		{
			int result;

			lock.ReadLock();
			result=FixedList<T>::Find(data);
			lock.ReadUnlock();

			return(result);
		}

		virtual bool SafeDelete(int n)
		{
			bool result;

			lock.WriteLock();
			result=FixedList<T>::Delete(n);
			lock.WriteUnlock();

			return(result);
		}

		virtual bool SafeDelete(const T &data)
		{
			int index;
			bool result;

			lock.WriteLock();
			index=FixedList<T>::Find(data);
			result=FixedList<T>::Delete(index);
			lock.WriteUnlock();

			return(result);
		}

		virtual void SafeDelete(const T *data,int n)
		{
			lock.WriteLock();
			FixedList<T>::Delete(data,n);
			lock.WriteUnlock();
		}

		virtual void SafeClear()
		{
			lock.WriteLock();
			FixedList<T>::Clear();
			lock.WriteUnlock();
		}

		virtual T SafeGetItem(int n)
		{
			T result;

			lock.ReadLock();
			result=FixedList<T>::operator[](n);
			lock.ReadUnlock();

			return result;
		}
	};//template<typename T> class MTFixedList

	template<typename T> class MTFixedObjectList:public MTFixedList<T *>							///<多线程定长对象列表
	{
	public:

		using MTFixedList<T *>::MTFixedList;
		virtual ~MTFixedObjectList()
		{
			MTFixedList<T *>::SafeClear();
		}

		virtual bool Delete(int);
		virtual void Clear();
	};//template<typename T> class MTFixedObjectList
}//namespace hgl
#include<hgl/type/FixedList.cpp>
#endif//HGL_FIXEDLIST_INCLUDE
