#ifndef HGL_RWLOCK_INCLUDE
#define HGL_RWLOCK_INCLUDE

#include<hgl/platform/Platform.h>
namespace hgl
{
	/**
	* 读写锁/共享锁
	*/
	class RWLock																					///读写锁/共享锁
	{
		void *lock;

	public:

		RWLock();
		virtual ~RWLock();

		void *GetRWLock(){ return lock; }															///<返回操作系级锁

		bool TryReadLock();																			///<尝试读(共享访问)锁定
		bool ReadLock();																			///<读(共享访问)锁定
		bool ReadUnlock();																			///<读(共享访问)解锁

		bool TryWriteLock();																		///<尝试写(独占访问)锁定
		bool WriteLock();																			///<写(独占访问)锁定
		bool WriteUnlock();																			///<写(独占访问)解锁

#if (HGL_OS != HGL_OS_Windows)&&(HGL_OS != HGL_OS_macOS)
		bool WaitReadLock(double);																	///<等待读(共享访问)锁定
		bool WaitWriteLock(double);																	///<等待写(独占访问)锁定
#endif//(HGL_OS != HGL_OS_Windows)&&(HGL_OS != HGL_OS_macOS)
	};//class RWLock

	#define HGL_RWLOCK(lock)				bool ReadLock()		{return lock.ReadLock();	}	\
											bool WriteLock()	{return lock.WriteLock();	}	\
											bool TryReadLock()	{return lock.TryReadLock();	}	\
											bool TryWriteLock()	{return lock.TryWriteLock();}	\
											bool ReadUnlock()	{return lock.ReadUnlock();	}	\
											bool WriteUnlock()	{return lock.WriteUnlock();	}

	#define HGL_RWLOCK_PTR(lock)			bool ReadLock()		{return lock->ReadLock();		}	\
											bool WriteLock()	{return lock->WriteLock();		}	\
											bool TryReadLock()	{return lock->TryReadLock();	}	\
											bool TryWriteLock()	{return lock->TryWriteLock();	}	\
											bool ReadUnlock()	{return lock->ReadUnlock();		}	\
											bool WriteUnlock()	{return lock->WriteUnlock();	}

	#define HGL_NULL_RWLOCK					bool ReadLock()		{return true;}	\
											bool WriteLock()	{return true;}	\
											bool TryReadLock()	{return true;}	\
											bool TryWriteLock()	{return true;}	\
											bool ReadUnlock()	{return true;}	\
											bool WriteUnlock()	{return true;}	\

	#define HGL_VIRTUAL_NULL_RWLOCK	virtual bool ReadLock()		{return true;}	\
									virtual bool WriteLock()	{return true;}	\
									virtual bool TryReadLock()	{return true;}	\
									virtual bool TryWriteLock()	{return true;}	\
									virtual bool ReadUnlock()	{return true;}	\
									virtual bool WriteUnlock()	{return true;}	\

	/**
	 * 读写锁/共享锁对象
	 */
	template<typename T> class RWLockObject:public RWLock
	{
		T *data;

	public:

		RWLockObject()
		{
			data=new T;
		}

		RWLockObject(T *t)
		{
			data=t;
		}

		virtual ~RWLockObject()
		{
			SAFE_CLEAR(data);
		}

		void operator = (T *t)
		{
			if(data)
				delete data;

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
	};//class RWLockObject

	/**
	 * 读写锁共享锁定自动释放类
	 */
	class OnlyReadLock
	{
		RWLock *rw;

	public:

		OnlyReadLock(RWLock &rw_lock)
		{
			rw=&rw_lock;

			rw->ReadLock();
		}

		OnlyReadLock(RWLock *rw_lock)
		{
			rw=rw_lock;

			rw->ReadLock();
		}

		~OnlyReadLock()
		{
			Unlock();
		}

		void Unlock()
		{
			if(rw)
			{
				rw->ReadUnlock();
				rw=nullptr;
			}
		}
	};//class ReadLock

	/**
	 * 读写锁独占访问锁定自动释放类
	 */
	class OnlyWriteLock
	{
		RWLock *rw;

	public:

		OnlyWriteLock(RWLock &rw_lock)
		{
			rw=&rw_lock;

			rw->WriteLock();
		}

		OnlyWriteLock(RWLock *rw_lock)
		{
			rw=rw_lock;

			rw->WriteLock();
		}

		~OnlyWriteLock()
		{
			Unlock();
		}

		void Unlock()
		{
			if(rw)
			{
				rw->WriteUnlock();
				rw=nullptr;
			}
		}
	};//class WriteLock
}//namespace hgl
#endif//HGL_RWLOCK_INCLUDE
