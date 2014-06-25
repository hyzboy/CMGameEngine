#ifndef HGL_SHARED_MEMORY_INCLUDE
#define HGL_SHARED_MEMORY_INCLUDE

#include<hgl/type/DataType.h>
namespace hgl
{
	/**
	* 共享内存
	*/
	class SharedMemory																				///共享内存
	{
		void *data;
		size_t size;

	public:

		SharedMemory();
		~SharedMemory();

		void *Create(int,char *name=0);																///<创建共享内存
		void *Attach(char *);																		///<关联共享内存
		void Detach();																				///<解除共享内存关联
		void Destroy();																				///<销毁共享内存

		void *GetAddress()const;																	///<取得共享内存地址
		size_t GetSize()const{return size;}															///<取得共享内存容量
	};//class SharedMemory
}//namespace hgl
#endif//HGL_SHARED_MEMORY_INCLUDE
