#ifndef HGL_THREAD_MUTEX_INCLUDE
#define HGL_THREAD_MUTEX_INCLUDE

#include<hgl/platform/Platform.h>
namespace hgl
{
	/**
	* 排斥(线程)访问类可以判断数据在被一个代码段访问时，不能被另一代码段访问<br>
    * 注：此排斥仅在当前进程内有效
	*/
	class ThreadMutex                                                                               ///排斥访问类(仅当前进程)
	{
		hgl_thread_mutex ptr;

	public:

		ThreadMutex();                                                                              ///<本类构造函数
		virtual ~ThreadMutex();                                                                     ///<本类析构函数

		hgl_thread_mutex *GetThreadMutex(){return &ptr;}

		virtual void	Lock();																		///<取得的控制权(如果对象处于排斥状态，则等待)
		virtual bool	TryLock();																	///<尝试取得控制权
		virtual bool	WaitLock(double=0);															///<等待并取得控制权
		virtual void	Unlock();																	///<放弃控制权
	};//class Mutex

	/**
	 * 线程排斥对象
	 */
	template<typename T> class ThreadMutexObject:public ThreadMutex
	{
		T *data;

	public:

		ThreadMutexObject()
		{
			data=new T;
		}

		ThreadMutexObject(T *t)
		{
			data=t;
		}

		virtual ~ThreadMutexObject()
		{
			SAFE_CLEAR(data);
		}

		void operator = (T *t)
		{
			data=t;
		}

		T *operator ->()
		{
			return data;
		}

		bool valid()const
		{
			return data;
		}

		bool operator !()
		{
			return !data;
		}
	};//class ThreadMutexObject

	class ThreadMutexLock
	{
 		ThreadMutex *tm;

	public:

		ThreadMutexLock(ThreadMutex *tm_lock)
		{
			tm=tm_lock;

			if(tm)
				tm->Lock();
		}

		~ThreadMutexLock()
		{
			if(tm)
				tm->Unlock();
		}

		void Lock()
		{
			if(tm)
				tm->Lock();
		}

		void Unlock()
		{
			if(tm)
				tm->Unlock();
		}

		void Clear()
		{
			if(tm)
			{
				tm->Unlock();
				tm=nullptr;
			}
		}
	};//class ThreadMutexLock
}//namespace hgl
#endif//HGL_THREAD_MUTEX_INCLUDE
