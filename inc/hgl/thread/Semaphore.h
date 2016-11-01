#ifndef HGL_SEMAPHORE_INCLUDE
#define HGL_SEMAPHORE_INCLUDE

namespace hgl
{
	/**
	* 信号是用于多线程同步中常用的一种技术<br>
	* 注意信号的用法是要有信号释放出来，才能获取到信号。
	*/
    class Semaphore                                                                                 ///信号
	{
		void *ptr;

	public:

		Semaphore(int=0);
		virtual ~Semaphore();

		virtual bool Release(int n=1);                                                              ///<释放信号

		virtual bool TryAcquire();                                                                  ///<尝试取得一个信号
		virtual bool Acquire(double time=0.0);														///<等待并获取一个信号
	};//class Semaphore
}//namespace hgl
#endif//HGL_SEMAPHORE_INCLUDE
