#include<hgl/proc/ProcMutex.h>
#include<apr_proc_mutex.h>

namespace hgl
{
	apr_pool_t *get_default_apr_pool();

	ProcMutex::ProcMutex()
	{
		lock=nullptr;
	}

	bool ProcMutex::Create(const char *name)
	{
		if(lock!=nullptr)return(false);

		if(apr_proc_mutex_create((apr_proc_mutex_t **)&lock,name,APR_LOCK_DEFAULT,get_default_apr_pool())==APR_SUCCESS)
			return(true);

		lock=nullptr;
		return(false);
	}

	void ProcMutex::Clear()
	{
		if(lock==nullptr)return;

		apr_proc_mutex_destroy((apr_proc_mutex_t *)lock);

		lock=nullptr;
	}

	bool ProcMutex::Lock()
	{
		if(lock==nullptr)return(false);

		return apr_proc_mutex_lock((apr_proc_mutex_t *)lock)==APR_SUCCESS;
	}

	bool ProcMutex::TryLock()
	{
		if(lock==nullptr)return(false);

		return apr_proc_mutex_trylock((apr_proc_mutex_t *)lock)==APR_SUCCESS;
	}

	bool ProcMutex::Unlock()
	{
		if(lock==nullptr)return(false);
		
		return apr_proc_mutex_unlock((apr_proc_mutex_t *)lock)==APR_SUCCESS;
	}
}//namespace hgl
