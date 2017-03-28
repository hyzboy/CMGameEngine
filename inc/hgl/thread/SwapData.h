#ifndef HGL_THREAD_SWAP_DATA_INCLUDE
#define HGL_THREAD_SWAP_DATA_INCLUDE

#include<hgl/thread/ThreadMutex.h>
#include<hgl/thread/Semaphore.h>

namespace hgl
{
    /**
     * 数据交换模板<br>
     * 使用环境为，后台多个线程进行阻塞式访问，而前台数据直接访问。前台在适当时机调用SWAP交换前后台数据
     */
    template<typename T> class SwapData
    {
    protected:

        T data[2];

        int back;
        int front;

        ThreadMutex lock;

    public:

        SwapData()
        {
            front=0;
            back=1;
        }

        virtual ~SwapData()=default;

        /**
         * 获取后台数据
         */
        bool GetBack(T &ptr)
        {
            if(!lock.Lock())
                return(false);

            ptr=data[back];

            return(true);
        }

        /**
         * 释放后台数据
         */
        void ReleaseBack()
        {
            lock.Unlock();
        }

        /**
         * 交换前后台数据
         */
        bool Swap()
        {
            if(!lock.Lock())
                return(false);

            if(front){front=0;back=1;}
                 else{front=1;back=0;}

            lock.Unlock();
            return(true);
        }
    };//template<typename T> class SwapData

	/**
	* 信号自动交换数据访问模板
	*/
	template<typename T> class SemSwapData:public SwapData<T>
	{
		Semaphore sem;

	public:

		using SwapData<T>::SwapData;
		~SemSwapData()=default;

		/**
		* 释放信号给这个模板
		* @param count 信号个数
		*/
		void Release(int count)
		{
			sem.Release(count);
		}

		/**
		* 等待获取一个信号并交换前后台数据
		* @param time_out 等待时长
		*/
		bool WaitSwap(const double time_out=5)
		{
			if(!sem.Acquire(time_out))
				return(false);

			Swap();
			return(true);
		}

		/**
		* 尝试获取一个信号并交换前后台数据
		*/
		bool TrySwap()
		{
			if(!sem.TryAcquire())
				return(false);

			Swap();
			return(true);
		}
	};//template<typename T> class SemSwapData:public SwapData<T>
}//namespace hgl
#endif//HGL_THREAD_SWAP_DATA_INCLUDE
