#include<hgl/thread/RWLock.h>
#include<hgl/MemoryPool.h>
#include<apr_thread_rwlock.h>

namespace hgl
{
	apr_pool_t *get_default_apr_pool();

	namespace apr
	{
		class RWLockAPR:public RWLock
		{
			apr_thread_rwlock_t *lock;

		public:

			RWLockAPR()
			{
				apr_thread_rwlock_create(&lock,get_default_apr_pool());
			}

			~RWLockAPR()
			{
				apr_thread_rwlock_destroy(lock);
			}

			void *GetRWLock()	{return lock;}

			bool TryReadLock()	{return apr_thread_rwlock_tryrdlock(lock)==APR_SUCCESS;}
			void ReadLock()		{apr_thread_rwlock_rdlock(lock);}
			void ReadUnlock()	{apr_thread_rwlock_unlock(lock);}

			bool TryWriteLock()	{return apr_thread_rwlock_trywrlock(lock)==APR_SUCCESS;}
			void WriteLock()	{apr_thread_rwlock_wrlock(lock);}
			void WriteUnlock()	{apr_thread_rwlock_unlock(lock);}
		};//class RWLockAPR
	}//namespace apr

	RWLock *CreateRWLockAPR()
	{
		return(new apr::RWLockAPR);
	}
}//namespace hgl