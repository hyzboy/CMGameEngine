#ifndef HGL_THREAD_DATA_POST_INCLUDE
#define HGL_THREAD_DATA_POST_INCLUDE

#include<hgl/thread/ThreadMutex.h>
#include<hgl/thread/Semaphore.h>

namespace hgl
{
    /**
     * 数据多线程投递模板<br>
     * 适用环境为一方线程接收，一方多线程投递。接收方可以在适当时候交换前后台数据
     */
    template<typename T> class DataPost
    {
    protected:

        T data[2];

        int post;
        int receive;

        ThreadMutex p_lock;
        ThreadMutex r_lock;

	protected:

		void _Swap()
		{
            if(receive){receive=0;post=1;}
                   else{receive=1;post=0;}
		}

    public:

        DataPost()
        {
            receive=0;
            post=1;
        }

        virtual ~DataPost()=default;

        /**
         * 交换双方数据
         */
        void Swap()
        {
            p_lock.Lock();
            r_lock.Lock();

            this->_Swap();

            r_lock.Unlock();
            p_lock.Unlock();
        }

        /**
         * 获取投递区的数据访问权
         */
        bool AcquirePost(const double &time_out=0)
        {
            if(time_out==0)
            {
                p_lock.Lock();
                return(true);
            }
            else
            {
                return p_lock.WaitLock(time_out);
            }
        }

        /**
         * 尝试获取投递区的数据访问权
         */
        bool TryAcquirePost()
        {
            return p_lock.TryLock();
        }

        /**
         * 获取投递区数据
         */
        T &GetPost()
        {
            return data[post];
        }

        /**
         * 获取接收区的数据访问权
         */
        bool AcquireRecv(const double &time_out=0)
        {
            if(time_out==0)
            {
                r_lock.Lock();
                return(true);
            }
            else
            {
                return r_lock.WaitLock(time_out);
            }
        }

        /**
         * 尝试获取接收区的数据访问权
         */
        bool TryAcquireRecv()
        {
            return r_lock.TryLock();
        }

        /**
         * 获取接收区数据
         */
        T &GetRecv()
        {
            return data[recvive];
        }
    };//template<typename T> class DataPost

	/**
	* 信号自动交换数据访问模板
	*/
	template<typename T> class SemDataPost:public DataPost<T>
	{
		Semaphore sem;

	public:

		using DataPost<T>::DataPost;
		~SemDataPost()=default;

		/**
		* 释放信号给这个模板
		* @param count 信号个数
		*/
		void Release(int count)
		{
			sem.Release(count);
		}

		/**
		* 等待获取一个信号并
		* @param time_out 等待时长
		*/
		bool Acquire(const double time_out=5)
		{
			return sem.Acquire(time_out))
		}

		/**
		* 尝试获取一个信号
		*/
		bool TryAcquire()
		{
			return sem.TryAcquire())
		}
	};//template<typename T,typename DP> class SemDataPost:public DP<T>
}//namespace hgl
#endif//HGL_THREAD_DATA_POST_INCLUDE
