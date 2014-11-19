#ifndef HGL_REDIS_DB_SAVE_THREAD_INCLUDE
#define HGL_REDIS_DB_SAVE_THREAD_INCLUDE

#include<hgl/thread/Thread.h>
#include<hgl/thread/CondVar.h>
#include<hgl/db/RedisDB.h>

namespace hgl
{
	namespace redis
	{
		/**
		 * RedisDB保存数据专用线程<br>
		 * 由于这里使用批量提交操作，所以仅可用于各种无需立即处理的数据保存
		 */
		class RedisDBSaveThread:public Thread
		{
			ThreadMutex tm;
			//CondVar cv;
			Semaphore sem;

			RedisDB *rdb;

			int count;
			int max_count;
			double time_out;

		public:

			RedisDBSaveThread()
			{
				rdb=nullptr;
				count=0;
			}

			~RedisDBSaveThread()
			{
				SAFE_CLEAR(rdb);
			}

			/**
			 * 初始化RedisDB
			 * @param db RedisDB指针
			 * @param mc 强制提交的数量
			 * @param to 强制提交的时间
			 */
			void Init(RedisDB *db,const int mc=100,const double to=10)
			{
				rdb=db;

				rdb->Multi();

				count=0;
				max_count=mc;
				time_out=to;
			}

			RedisDB *Get()
			{
				tm.Lock();
				return rdb;
			}

			RedisDB *TryGet()
			{
				if(!tm.TryLock())
					return(nullptr);

				return rdb;
			}

			void Release()
			{
				int result=++count;

				tm.Unlock();

				if(result>=max_count)				//强制提交一回
					//cv.Signal();
					sem.Release();
			}

			bool Execute()
			{
				//if(!cv.Wait(&tm,time_out))
				sem.Acquire(time_out);

				tm.Lock();						//如果没有等到信号，则自行锁定

				if(count>0)
				{
					rdb->Exec();
					rdb->Multi();
					count=0;
				}

				tm.Unlock();
				return(true);
			}
		};//class RedisDBSaveThread
	}//namespace redis
}//namespace hgl
#endif//HGL_REDIS_DB_SAVE_THREAD_INCLUDE
