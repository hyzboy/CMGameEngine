#include<hgl/MemoryPool.h>
#include<apr_pools.h>

namespace hgl
{
	static apr_pool_t *apr_default_memory_pool=nullptr;

	void InitMemoryPool()
	{
		apr_pool_initialize();
		apr_pool_create(&apr_default_memory_pool,nullptr);
	}

	void ClearMemoryPool()
	{
		if(!apr_default_memory_pool)return;

		apr_pool_destroy(apr_default_memory_pool);
		apr_default_memory_pool=nullptr;
		apr_pool_terminate();
	}

	apr_pool_t *get_default_apr_pool()
	{
		return apr_default_memory_pool;
	}
}//namespace hgl

namespace hgl
{
	MemoryPool::MemoryPool(MemoryPool *mp)
	{
		if(mp)
			parent=mp->pool;
		else
			parent=nullptr;

		apr_pool_create(&pool,parent);
	}

	MemoryPool::~MemoryPool()
	{
		apr_pool_destroy(pool);
	}

	void *MemoryPool::malloc(int size)
	{
		return apr_palloc(pool,size);
	}

	void *MemoryPool::calloc(int size)
	{
		return apr_pcalloc(pool,size);
	}

	void MemoryPool::free(void *ptr)
	{
		apr_pool_cleanup_null(ptr);
	}

	void MemoryPool::free_all()
	{
		apr_pool_clear((apr_pool_t *)pool);
	}
}//namespace hgl
