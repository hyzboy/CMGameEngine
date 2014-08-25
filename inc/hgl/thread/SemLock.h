#ifndef HGL_THREAD_SEM_LOCK_INCLUDE
#define HGL_THREAD_SEM_LOCK_INCLUDE

#include<hgl/thread/Semaphore.h>
#include<hgl/thread/ThreadMutex.h>
#include<hgl/thread/RWLock.h>
namespace hgl
{
	template<typename T,typename L> class SemLockTemplate
	{
	protected:

		T data;

		L lock;
		Semaphore sem;

	public:

		SemLockTemplate(){}

		/**
		 * 本类构造函数
		 * @param d 数据
		 * @param sem_count 最大信号数量(默认0表示不限制)
		 */
		SemLockTemplate(T &d,int sem_count=0):sem(sem_count)
		{
			data=d;
		}

	public:	//数据相关

		void operator = (T &d){data=d;}
		T *operator->(){return &data;}

	public:	//信号相关

		/**
		 * 释放信号
		 * @param n 释放的信号数量
		 * @return 是否成功
		 */
		bool Release(int n=1)
		{
			if(n<=0)return(false);
			return sem.Release(n);
		}

		bool TryAcquire(){return sem.TryAcquire();}													///<尝试获取一个信号
		/**
		 * 获取一个信号
		 * @param time_out 超时时间
		 * @return 是否成功
		 */
		bool Acquire(double time_out=0.0f){return sem.Acquire();}
	};//template<typename T,typename L> class SemLockTemplate

	/**
	 * 信号线程排斥
	 */
	template<typename T> class SemThreadMutex:public SemLockTemplate<T,ThreadMutex>
	{
	public:

		using SemLockTemplate<T,ThreadMutex>::SemLockTemplate;

	public:	//线程排斥相关

		void	Lock(){this->lock.Lock();}															///<取得的控制权(如果对象处于排斥状态，则等待)
		bool	TryLock(){return this->lock.TryLock();}												///<尝试取得控制权
		bool	WaitLock(double time_out=0){return this->lock.WaitLock();}							///<等待并取得控制权
		void	Unlock(){this->lock.Unlock();}														///<放弃控制权

	public:	//综合应用相关

		/**
		 * 在指定时间内如果成功捕获到信号则锁定
		 * @param time_out 最长等待时间
		 * @return 是否锁定了
		 */
		bool	WaitSemLock(double time_out)
		{
			if(!this->sem.Acquire(time_out))
				return(false);

			this->lock.Lock();
			return(true);
		}

		/**
		 * 尝试捕获一个信号，如果成功则锁定
		 * @return 是否锁定了
		 */
		bool	TrySemLock()
		{
			if(!this->sem.TryAcquire())
				return(false);

			this->lock.Lock();
			return(true);
		}

		/**
		 * 锁定
		 * @param try_lock 是否是尝试锁定
		 * @param time_out 如果不是尝试锁定，则等待锁定的超时时间
		 */
		bool SemLock(bool try_lock,double time_out)
		{
			if(try_lock)
			{
				if(!this->sem.TryAcquire())
					return(false);
			}
			else				//当前没人，那就慢慢等吧
			{
				if(!this->sem.Acquire(time_out))
					return(false);
			}

			this->lock.Lock();
			return(true);
		}

		/**
		 * 解锁并释放信号
		 * @param n 释放的信号数量
		 */
		void	SemUnlock(int n=1)
		{
			this->lock.Unlock();

			if(n>0)
			this->sem.Release(n);
		}
	};//template<typename T> class SemThreadMutex

	/**
	 * 信号读写锁
	 */
	template<typename T> class SemRWLock:public SemLockTemplate<T,RWLock>
	{
	public:

		using SemLockTemplate<T,RWLock>::SemLockTemplate;

	public:	//读写锁相关

		HGL_RWLOCK(this->lock);

	public:	//综合应用相关

		/**
		 * 在指定时间内如果成功捕获到信号则共享锁定
		 * @param time_out 最长等待时间
		 * @return 是否锁定了
		 */
		bool	WaitSemReadLock(double time_out)
		{
			if(!this->sem.Acquire(time_out))
				return(false);

			this->lock.ReadLock();
			return(true);
		}

		/**
		 * 在指定时间内如果成功捕获到信号则独占锁定
		 * @param time_out 最长等待时间
		 * @return 是否锁定了
		 */
		bool	WaitSemWriteLock(double time_out)
		{
			if(!this->sem.Acquire(time_out))
				return(false);

			this->lock.WriteLock();
			return(true);
		}

		/**
		 * 尝试捕获一个信号，如果成功则共享锁定
		 * @return 是否锁定了
		 */
		bool	TrySemReadLock()
		{
			if(!this->sem.TryAcquire())
				return(false);

			this->lock.ReadLock();
			return(true);
		}

		/**
		 * 尝试捕获一个信号，如果成功则独占锁定
		 * @return 是否锁定了
		 */
		bool	TrySemWriteLock()
		{
			if(!this->sem.TryAcquire())
				return(false);

			this->lock.WriteLock();
			return(true);
		}

		/**
		 * 解除共享锁并释放信号
		 * @param n 释放的信号数量
		 */
		void	SemReadUnlock(int n=1)
		{
			this->lock.ReadUnlock();

			if(n>0)
			this->sem.Release(n);
		}

		/**
		 * 解除独占锁并释放信号
		 * @param n 释放的信号数量
		 */
		void	SemWriteUnlock(int n=1)
		{
			this->lock.WriteUnlock();

			if(n>0)
			this->sem.Release(n);
		}
	};//template<typename T> class SemRWLock
}//namespace hgl
#endif//HGL_THREAD_SEM_LOCK_INCLUDE
