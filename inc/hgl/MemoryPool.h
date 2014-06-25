#ifndef HGL_MEMORY_POOL_INCLUDE
#define HGL_MEMORY_POOL_INCLUDE

struct apr_pool_t;

namespace hgl
{
	/**
	* 内存池<br>
	* 使用Apache内存池技术实现
	*/
	class MemoryPool
	{
		apr_pool_t *pool;
		apr_pool_t *parent;

	public:

		MemoryPool(MemoryPool *mp=0);
		~MemoryPool();

		apr_pool_t *get_apr_pool(){return pool;}													///<取得apr pool

		void *malloc(int);																			///<分配内存
		void *calloc(int);																			///<分配内存并从清0
		void free(void *);																			///<释放内存

		void free_all();																			///<清除池中所有分配的内存
	};//class MemoryPool
}//namespace hgl
#endif//HGL_MEMORY_POOL_INCLUDE
