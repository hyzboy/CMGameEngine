#include<hgl/thread/RWLock.h>
#include<pthread.h>

namespace hgl
{
	void GetWaitTime(struct timespec &,double);

	RWLock::RWLock()
	{
		lock=new pthread_rwlock_t;

		pthread_rwlock_init((pthread_rwlock_t *)lock,nullptr);
	}

	RWLock::~RWLock()
	{
		pthread_rwlock_destroy((pthread_rwlock_t *)lock);
	}

	bool RWLock::TryReadLock()	{return !pthread_rwlock_tryrdlock((pthread_rwlock_t *)lock);}
	bool RWLock::WaitReadLock(double t)
	{
		struct timespec abstime;

		GetWaitTime(abstime,t);

		return !pthread_rwlock_timedrdlock((pthread_rwlock_t *)lock,&abstime);
	}

	bool RWLock::ReadLock()		{return !pthread_rwlock_rdlock((pthread_rwlock_t *)lock);}
	bool RWLock::ReadUnlock()	{return !pthread_rwlock_unlock((pthread_rwlock_t *)lock);}

	bool RWLock::TryWriteLock()	{return !pthread_rwlock_trywrlock((pthread_rwlock_t *)lock);}
	bool RWLock::WaitWriteLock(double t)
	{
		struct timespec abstime;

		GetWaitTime(abstime,t);

		return !pthread_rwlock_timedwrlock((pthread_rwlock_t *)lock,&abstime);
	}

	bool RWLock::WriteLock()	{return !pthread_rwlock_wrlock((pthread_rwlock_t *)lock);}
	bool RWLock::WriteUnlock()	{return !pthread_rwlock_unlock((pthread_rwlock_t *)lock);}
}//namespace hgl
