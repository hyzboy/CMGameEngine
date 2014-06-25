#ifndef HGL_PROCESS_MUTEX_INCLUDE
#define HGL_PROCESS_MUTEX_INCLUDE

namespace hgl
{
	/**
	* 进程排斥
	*/
	class ProcMutex																					///进程排斥
	{
		void *lock;

	public:

		ProcMutex();
		~ProcMutex(){Clear();}

		bool Create(const char *);																	///<创建进程排斥
		void Clear();																				///<清除进程排斥

		bool Lock();																				///<锁定
		bool TryLock();																				///<尝试锁定

		bool Unlock();																				///<解锁
	};//class ProcMutex
}//namespace hgl
#endif//HGL_PROCESS_MUTEX_INCLUDE
