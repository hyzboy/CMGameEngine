#ifndef HGL_THREAD_SWAP_LIST_INCLUDE
#define HGL_THREAD_SWAP_LIST_INCLUDE

#include<hgl/thread/ThreadMutex.h>
#include<hgl/thread/Semaphore.h>
#include<hgl/type/List.h>
#include<hgl/type/Set.h>
namespace hgl
{
	/**
	 * 数据交换合集
	 */
	template<typename T,template<typename> class S> class SwapColl
	{
	protected:

		S<T> join_list;
		S<T> proc_list;

		ThreadMutex lock;

	public:

		virtual ~SwapColl(){}

		/**
		 * 加入一个数据到合集中
		 */
		bool Add(T &item)
		{
			lock.Lock();
			int result=join_list.Add(item);
			lock.Unlock();

			return(result!=-1);
		}

		/**
		 * 增加一批数据到合集中
		 */
		int Add(T *item_list,const int count)
		{
			lock.Lock();
			int result=join_list.Add(item_list,count);
			lock.Unlock();

			return result;
		}

		/**
		 * 取得可以使用的列表
		 */
		S<T> &GetProcList()
		{
			lock.Lock();
				proc_list.Add(join_list);
				join_list.ClearData();
			lock.Unlock();

			return proc_list;
		}

		S<T> *operator ->(){return &proc_list;}
		operator S<T> &(){return proc_list;}
	};//template<typename T> class SwapColl

	template<typename T> using SwapList=SwapColl<T,List>;		///<安全交换列表
	template<typename T> using SwapSet=SwapColl<T,Set>;			///<安全交换集合

	/**
	 * 信号安全交换合集
	 */
	template<typename T,template<typename> class S> class SemSwapColl
	{
	protected:

		S<T> join_list;
		S<T> proc_list;

		ThreadMutex lock;
		Semaphore sem;

	public:

		virtual ~SemSwapColl()=default;

		/**
		 * 增加一个数据到合集中
		 */
		bool Add(T &item)
		{
			lock.Lock();
			int result=join_list.Add(item);
			lock.Unlock();

			if(result!=-1)
				sem.Release(1);

			return(result!=-1);
		}

		/**
		 * 增加一批数据到合集中
		 */
		int Add(T *item_list,const int count)
		{
			lock.Lock();
			int result=join_list.Add(item_list,count);
			lock.Unlock();

			if(result>0)
				sem.Release(1);

			return result;
		}

		/**
		 * 等待信号
		 */
		bool WaitProc(const double time_out=HGL_TIME_ONE_MINUTE)
		{
			if(!sem.Acquire(time_out))
				return(false);

			lock.Lock();
				proc_list.Add(join_list);
				join_list.ClearData();
			lock.Unlock();

			return(true);
		}

		/**
		 * 等待信号
		 */
		bool TryProc()
		{
			if(!sem.TryAcquire())
				return(false);

			lock.Lock();
				proc_list.Add(join_list);
				join_list.ClearData();
			lock.Unlock();

			return(true);
		}

		S<T> *operator ->(){return &proc_list;}
		operator S<T> &(){return proc_list;}
	};//template<typename T> class SemSwapColl

	template<typename T> using SemSwapList=SemSwapColl<T,List>;		///<安全信号交换列表
	template<typename T> using SemSwapSet=SemSwapColl<T,Set>;		///<安全信号交换集合
}//namespace hgl
#endif//HGL_THREAD_SWAP_LIST_INCLUDE
