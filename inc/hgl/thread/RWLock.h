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
//		bool WaitReadLock(double);																	///<等待读(共享访问)锁定
		bool ReadLock();																			///<读(共享访问)锁定
		bool ReadUnlock();																			///<读(共享访问)解锁

		bool TryWriteLock();																		///<尝试写(独占访问)锁定
//		bool WaitWriteLock(double);																	///<等待写(独占访问)锁定
		bool WriteLock();																			///<写(独占访问)锁定
		bool WriteUnlock();																			///<写(独占访问)解锁
	};//class RWLock

	RWLock *CreateRWLock()																			///<创建读写锁
	{
		return(new RWLock);
	}

	#define HGL_RWLOCK(lock)	void ReadLock()		{lock->ReadLock();		}	\
								void WriteLock()	{lock->WriteLock();		}	\
								bool TryReadLock()	{return lock->TryReadLock();	}	\
								bool TryWriteLock()	{return lock->TryWriteLock();	}	\
								void ReadUnlock()	{lock->ReadUnlock();	}	\
								void WriteUnlock()	{lock->WriteUnlock();	}

	/**
	 * 读写锁共享锁定自动释放类
	 */
	class OnlyReadLock
	{
		RWLock *rw;

	public:

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
