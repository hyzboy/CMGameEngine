#include<hgl/thread/RWLock.h>
#include<pthread.h>

namespace hgl
{
	void GetWaitTime(struct timespec &,double);

	class CreateRWLockPOSIX:public RWLock
	{
		pthread_rwlock_t lock;

	public:

		CreateRWLockPOSIX()
		{
			pthread_rwlock_init(&lock,nullptr);
		}

		~CreateRWLockPOSIX()
		{
			pthread_rwlock_destroy(&lock);
		}

		void *GetRWLock()	{return &lock;}

		bool TryReadLock()	{return !pthread_rwlock_tryrdlock(&lock);}
		bool WaitReadLock(double t)
		{
			struct timespec abstime;

			GetWaitTime(abstime,t);

			return !pthread_rwlock_timedrdlock(&lock,&abstime);
		}

		bool ReadLock()		{!pthread_rwlock_rdlock(&lock);}
		bool ReadUnlock()	{!pthread_rwlock_unlock(&lock);}

		bool TryWriteLock()	{return !pthread_rwlock_trywrlock(&lock);}
		bool WaitWriteLock(double t)
		{
			struct timespec abstime;

			GetWaitTime(abstime,t);

			return !pthread_rwlock_timedwrlock(&lock,&abstime);
		}

		bool WriteLock()	{!pthread_rwlock_wrlock(&lock);}
		bool WriteUnlock()	{!pthread_rwlock_unlock(&lock);}
	};//class RWLockPOSIX

	RWLock *CreateRWLock()
	{
		return(new CreateRWLockPOSIX);
	}
}//namespace hgl
