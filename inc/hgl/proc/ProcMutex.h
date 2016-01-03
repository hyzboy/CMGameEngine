#ifndef HGL_PROCESS_MUTEX_INCLUDE
#define HGL_PROCESS_MUTEX_INCLUDE

#include<hgl/type/DataType.h>

#if HGL_OS!=HGL_OS_Windows
    #include<semaphore.h>
#endif//HGL_OS!=HGL_OS_Windows

namespace hgl
{
	/**
	* 进程排斥
	*/
	class ProcMutex																					///进程排斥
	{
#if HGL_OS==HGL_OS_Windows
		void *lock;
#else
        sem_t *lock;
#endif//HGL_OS==HGL_OS_Windows

	public:

		ProcMutex();
		~ProcMutex(){Clear();}

		bool Create(const os_char *);																///<创建进程排斥
		void Clear();																				///<清除进程排斥

		bool Lock();																				///<锁定
		bool TryLock();																				///<尝试锁定

		bool Unlock();																				///<解锁
	};//class ProcMutex
}//namespace hgl
#endif//HGL_PROCESS_MUTEX_INCLUDE
