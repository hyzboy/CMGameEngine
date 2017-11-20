#ifndef HGL_THREAD_DATA_POST_INCLUDE
#define HGL_THREAD_DATA_POST_INCLUDE

#include<hgl/thread/ThreadMutex.h>
#include<hgl/thread/Semaphore.h>
#include<hgl/type/Stack.h>

namespace hgl
{
	/**
	 * 多线程数据投递模板<br>
	 * 需要注意每个任务都会重新由空闲线程来进行获取，所以请将连续的任务合并为一个任务，而不是一次大量投递。
	 */
	template<typename T> class DataPost
	{
	protected:

		List<T *> data_list[2];

		int post_index,recv_index;
		int recv_offset;

		ThreadMutex post_lock,recv_lock;

	protected:

		void _Swap()
		{
            if(recv_index){recv_index=0;post_index=1;}
					  else{recv_index=1;post_index=0;}

			recv_offset=0;
		}

	public:

		DataPost()
		{
			post_index=0;
			recv_index=1;

			recv_offset=0;
		}

		virtual ~DataPost()=default;

		/**
		 * 投递一个数据
		 */
		void Post(T *obj)
		{
			if(!obj)return;

			post_lock.Lock();
				data_list[post_index].Add(obj);
			post_lock.Unlock();
		}

		/**
		 * 投递一批数据
		 */
		void Post(T **obj,int count)
		{
			if(!obj)return;

			post_lock.Lock();
				data_list[post_index].Add(obj,count);
			post_lock.Unlock();
		}

    public:

		/**
		 * 获取一个数据
		 */
		T *Receive()
		{
			T *obj=nullptr;

			recv_lock.Lock();
				int count=data_list[recv_index].GetCount();

				if(recv_offset<count)
				{
					obj=*(data_list[recv_index].GetData()+recv_offset);

					++recv_offset;
				}
				else
				{
					data_list[recv_index].ClearData();		//清空接收区的数据

					post_lock.Lock();
					_Swap();
					post_lock.Unlock();

                    count=data_list[recv_index].GetCount();
                    if(count>0)                             //如果换出来的区有数据
                    {
                        obj=*(data_list[recv_index].GetData()+recv_offset);

                        ++recv_offset;
                    }
				}
			recv_lock.Unlock();

			return obj;
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
		* 释放接收信号
		* @param count 信号个数
		*/
		void PostSem(int count=1)
		{
			sem.Post(count);
		}

		/**
		* 等待获取一个信号并获取数据
		* @param time_out 等待时长
		*/
		T *WaitSemReceive(const double time_out=5)
		{
			if(!sem.Acquire(time_out))
				return(nullptr);

			return this->Receive();
		}

		/**
		* 尝试获取一个信号并获取数据
		*/
		T *TrySemReceive()
		{
			if(!sem.TryAcquire())
				return(nullptr);

			return this->Receive();
		}
	};//template<typename T> class SemDataPost:public DataPost<T>
}//namespace hgl
#endif//HGL_THREAD_DATA_POST_INCLUDE
