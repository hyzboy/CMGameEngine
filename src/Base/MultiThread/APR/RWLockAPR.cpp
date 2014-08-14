#include<hgl/thread/RWLock.h>
#include<hgl/MemoryPool.h>
#include<apr_thread_rwlock.h>

namespace hgl
{
	apr_pool_t *get_default_apr_pool();

	RWLock::RWLock()
	{
		apr_thread_rwlock_create((apr_thread_rwlock_t **)&lock, get_default_apr_pool());
	}

	RWLock::~RWLockAPR()
	{
		apr_thread_rwlock_destroy((apr_thread_rwlock_t *)lock);
	}
	
	bool RWLock::TryReadLock()	{ return apr_thread_rwlock_tryrdlock((apr_thread_rwlock_t *)lock) == APR_SUCCESS; }
	bool RWLock::ReadLock()		{ return apr_thread_rwlock_rdlock((apr_thread_rwlock_t *)lock) == APR_SUCCESS; }
	bool RWLock::ReadUnlock()	{ return apr_thread_rwlock_unlock((apr_thread_rwlock_t *)lock) == APR_SUCCESS; }

	bool RWLock::TryWriteLock()	{ return apr_thread_rwlock_trywrlock((apr_thread_rwlock_t *)lock) == APR_SUCCESS; }
	bool RWLock::WriteLock()	{ return apr_thread_rwlock_wrlock((apr_thread_rwlock_t *)lock) == APR_SUCCESS; }
	bool RWLock::WriteUnlock()	{ return apr_thread_rwlock_unlock((apr_thread_rwlock_t *)lock) == APR_SUCCESS; }
}//namespace hgl