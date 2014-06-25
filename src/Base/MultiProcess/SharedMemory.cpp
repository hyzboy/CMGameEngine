#include<hgl/proc/SharedMemory.h>
#include<apr_shm.h>
#include<hgl/LogInfo.h>

namespace hgl
{
	apr_pool_t *get_default_apr_pool();

	SharedMemory::SharedMemory()
	{
		data=nullptr;
		size=0;
	}

	SharedMemory::~SharedMemory()
	{
	}

	/**
	* 创建共享内存
	* @param sm_size 共享内存容量
	* @param sm_name 共享内存名称,可以不写
	* @return 分配好的共享内存指针
	* @return NULL 分配失败
	*/
	void *SharedMemory::Create(int sm_size,char *sm_name)
	{
		apr_status_t rv;

		apr_pool_t *pool=get_default_apr_pool();

		apr_shm_remove(sm_name,pool);

		rv=apr_shm_create((apr_shm_t **)&data,sm_size,sm_name,pool);

		if(rv==APR_SUCCESS)
		{
			size=sm_size;
			return(apr_shm_baseaddr_get((apr_shm_t *)data));
		}
		else
		{
			LOG_ERROR(u"Create SharedMemory error.");
			return(nullptr);
		}
	}

	/**
	* 关联共享内存
	* @param sm_name 共享内存名称
	* @return 共享内存访问地址
	* @return NULL 关联失败
	*/
	void *SharedMemory::Attach(char *sm_name)
	{
		apr_status_t rv;

		rv=apr_shm_attach((apr_shm_t **)&data,sm_name,get_default_apr_pool());

		if(rv==APR_SUCCESS)
		{
			size=apr_shm_size_get((apr_shm_t *)data);
			return(apr_shm_baseaddr_get((apr_shm_t *)data));
		}
		else
		{
			LOG_ERROR(u"Attach SharedMemory error.");
			return(nullptr);
		}
	}

	/**
	* 解除共享内存关联
	*/
	void SharedMemory::Detach()
	{
		if(!data)return;

		apr_shm_detach((apr_shm_t *)data);

		data=nullptr;
		size=0;
	}

	/**
	* 销毁当前共享内存
	*/
	void SharedMemory::Destroy()
	{
		if(!data)return;

#ifndef _DEBUG
		apr_shm_destroy((apr_shm_t *)data);
#else
		apr_status_t rv=apr_shm_destroy((apr_shm_t *)data);

		if(rv!=APR_SUCCESS)
			LOG_ERROR(u"Destroy SharedMemory error.");
#endif//_DEBUG

		data=nullptr;
		size=0;
	}

	/**
	* 取得共享内存地址
	* @return 共享内存地址
	* @return NULL 出错
	*/
	void *SharedMemory::GetAddress() const
	{
		if(data)
			return(apr_shm_baseaddr_get((apr_shm_t *)data));

		LOG_ERROR(u"Error of Get address from SharedMemory.");
		return(nullptr);
	}
}//namespace hgl
