#ifndef HGL_THREAD_INCLUDE
#define HGL_THREAD_INCLUDE

#include<hgl/type/DataType.h>
#include<hgl/thread/Semaphore.h>

#if HGL_OS == HGL_OS_Windows
	#include<windows.h>
	typedef HANDLE ThreadPtr;
#else
	#include<pthread.h>
	typedef pthread_t ThreadPtr;
#endif//HGL_OS == HGL_Windows
namespace hgl
{
	/**
	* 线程类.本类用于建立一个线程并管理它。<br>
	* Sleep和Exit函数是给线程内的程序使用的；而Start,Close,Pause,Rewind是给线程外的程序使用的。<br>
	* 使用本类只需要重载本类，并将代码写在Execute函数内即可。<br>
	* 执行大概为：<br>
	* if(ProcStartThread())		<br>
	* 	while(Execute());	<br>
	* else					<br>
	* 	return(0);
	* 						<br>
	* ProcEndThread();
	*/
	class Thread																					///线程类
	{
		friend void WaitThread(Thread **,int,double);

	protected:

		ThreadPtr threadptr;

#if HGL_OS != HGL_OS_Windows
		virtual void SetCancelState(bool,bool=true);
#endif//HGL_OS != HGL_OS_Windows

	public:

		Thread();
		virtual ~Thread();

		const uint64 GetThreadID()const;															///<取得线程ID

		/**
		* 线程执行函数，会被反复调用
		* @return true 继续执行
		* @return false 退出执行
		*/
		virtual bool Execute()=0;                                                                   ///<线程的运行函数

		virtual bool ProcStartThread(){return(true);}												///<线程启动运行函数,在Execute前被调用
		virtual void ProcEndThread(){};																///<结程结束运行函数,在Execute后被调用

		virtual bool IsExitDelete()const{return true;}												///<返回在退出线程时，是否删除本对象

	public:	//线程运行控制

		virtual bool Start();																		///<开始运行当前线程
		virtual void Close();                                                                       ///<强制关闭当前线程(其它线程调)

		virtual bool IsCurThread();																	///<是否是当前线程

#if HGL_OS != HGL_OS_Windows
		virtual bool Cancel();																		///<放弃这个线程
#endif//HGL_OS != HGL_OS_Windows

        /**
         * 等待当前线程结束<br>
         * 注：此函数在Win平台下任何情况下都起作用，但在linux/bsd下，如果IsExitDelete提供true的值，此函数将不起作用
         */
		virtual void Wait(double time=0);                                                 			///<等待当前线程
	};//class Thread

	void WaitThread(Thread **,int,double time=0);													///<等待多个线程中的一个完成

	bool CreateThread(Thread *);																	///<创建一个线程
}//namespace hgl
#endif//HGL_THREAD_INCLUDE
