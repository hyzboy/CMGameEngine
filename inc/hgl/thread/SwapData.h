#ifndef HGL_THREAD_SWAP_DATA_INCLUDE
#define HGL_THREAD_SWAP_DATA_INCLUDE

#include<hgl/thread/ThreadMutex.h>
#include<hgl/thread/Semaphore.h>

namespace hgl
{
    /**
     * 单向多线程数据交换模板<br>
     * 适用环境为一方单线程独占方，一方多线程共享访问方。独占方可以在适当时候交换前后台数据
     */
    template<typename T> class SingleSwapData
    {
    protected:

        T data[2];

        int shared;
        int exclusive;

        ThreadMutex lock;

	protected:

		void _Swap()
		{
            if(exclusive){exclusive=0;shared=1;}
                     else{exclusive=1;shared=0;}
		}

    public:

        SingleSwapData()
        {
            exclusive=0;
            shared=1;
        }

        virtual ~SingleSwapData()=default;

        /**
         * 获取共享的数据访问权
         */
        T &GetShared()
        {
            lock.Lock();

            return data[shared];
        }

        /**
         * 释放多线程共享的数据访问权
         */
        void ReleaseShared()
        {
            lock.Unlock();
        }

        /**
        * 取得独占数据访问权
        */
        T &GetExclusive(){return data[exclusive];}

        /**
         * 交换双方数据
         */
        void Swap()
        {
            lock.Lock();

            this->_Swap();

            lock.Unlock();
        }

		/**
		 * 尝试交换双方数据
		 */
		bool TrySwap()
		{
			if(!lock.TryLock())
				return(false);

			this->_Swap();

            lock.Unlock();
			return(true);
		}

		/**
		 * 尝试交换双方数据
		 */
		bool WaitSwap(const double &time_out)
		{
			if(!lock.WaitLock(time_out))
				return(false);

			this->_Swap();

            lock.Unlock();
			return(true);
		}
    };//template<typename T> class SingleSwapData

    /**
     * 多向数据交换访问模板<br>
     * 在多线程之间，每个线程都可以随意的访问指定的前后台2份数据
     */
    template<typename T> class MutliSwapData
    {
    };//template<typename T> class MutliSwapData

	/**
	* 信号自动交换数据访问模板
	*/
	template<typename T,typename SD> class SemSwapData:public SD<T>
	{
		Semaphore sem;

	public:

		using SD<T>::SwapData;
		~SemSwapData()=default;

		/**
		* 释放信号给这个模板
		* @param count 信号个数
		*/
		void ReleaseSem(int count)
		{
			sem.Release(count);
		}

		/**
		* 等待获取一个信号并交换前后台数据
		* @param time_out 等待时长
		*/
		bool WaitSemSwap(const double time_out=5)
		{
			if(!sem.Acquire(time_out))
				return(false);

			this->Swap();
			return(true);
		}

		/**
		* 尝试获取一个信号并交换前后台数据
		*/
		bool TrySemSwap()
		{
			if(!sem.TryAcquire())
				return(false);

			this->Swap();
			return(true);
		}
	};//template<typename T,typename SD> class SemSwapData:public SD<T>
}//namespace hgl
#endif//HGL_THREAD_SWAP_DATA_INCLUDE
