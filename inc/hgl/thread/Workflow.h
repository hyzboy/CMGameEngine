#ifndef HGL_WORKFLOW_INCLUDE
#define HGL_WORKFLOW_INCLUDE

#include<hgl/thread/Thread.h>
#include<hgl/thread/ThreadMutex.h>
#include<hgl/thread/SwapData.h>
#include<hgl/thread/DataPost.h>
#include<hgl/type/List.h>
namespace hgl
{
	/**
	 * 工作流名字空间<br>
	 * 工作流是一种对工作的多线安排管理机制，它适用于按业务分配多线程的工作环境<br>
	 * 开发者需要为每一种工作指定一定的线程数量，但每一种工作确只有一个工作分配入口和分发出口。<br>
	 * 由其它程序提交工作任务到入口，开发者可以自行重载分配入口的分配函数。
	 */
	namespace workflow
	{
		template<typename W> class WorkProc
		{
		public:

			virtual ~WorkProc()=default;

		public:	//投递工作线程所需调用的方法

			virtual void Post(W *w)=0;																		///<投递一个工作
			virtual void Post(W **w,int count)=0;															///<投递一批工作

		public:	//需用户重载实现的真正执行工作的方法

			/**
			 * 单个工作执行事件函数，此函数需用户重载实现
			 */
			virtual void OnWork(const uint,W *)=0;

		public:	//由工作线程调用的执行工作事件函数

			virtual bool OnExecuteWork(const uint)=0;
		};//template<typename W> class WorkProc

		/**
		 * 单体工作处理<br>
		 * 该类可以由多个线程投递工作，但只能被一个工作线程获取工作
		 */
		template<typename W> class SingleWorkProc:public WorkProc<W>
		{
		public:

			using WorkList=List<W *>;

		private:

			SemSwapData<WorkList> work_list;																///<工程列表

		protected:

			double time_out;

		public:

			SingleWorkProc()
			{
				time_out=5;
			}

			virtual ~SingleWorkProc()=default;

			void SetTimeOut(const double to)																///<设置超时时间
			{
				if(to<=0)time_out=0;
					else time_out=to;
			}

			virtual void Post(W *w)																			///<投递一个工作
			{
				WorkList &wl=work_list.GetPost();
					wl.Add(w);
				work_list.ReleasePost();
			}

			virtual void Post(W **w,int count)																///<投递一批工作
			{
				WorkList &wl=work_list.GetPost();
					wl.Add(w,count);
				work_list.ReleasePost();
			}

			virtual void ToWork()																			///<将堆积的工作列表发送给工作线程
			{
				work_list.ReleaseSem(1);
			}

		public:

			/**
			 * 当前工作序列完成事件函数，如需使用请重载此函数
			 */
			virtual void OnFinish(const uint wt_index)
			{
			}

			/**
			 * 开始执行工作函数
			 */
			virtual bool OnExecuteWork(const uint wt_index) override
			{
				if(!work_list.WaitSemSwap(time_out))
						return(false);

				WorkList &wl=work_list.GetReceive();

				const int count=wl.GetCount();

				if(count>0)
				{
					W **p=wl.GetData();

					for(int i=0;i<count;i++)
					{
						this->OnWork(wt_index,*p);
						++p;
					}

					this->OnFinish(wt_index);

					wl.ClearData();
				}

				return(true);
			}
		};//template<typename W> class SingleWorkProc:public WorkProc<W>

		/**
		 * 多体工作处理<br>
		 * 该类可以由多个线程投递工作，也可以同时被多个工作线程获取工作
		 */
		template<typename W> class MultiWorkProc:public WorkProc<W>
		{
		protected:

			SemDataPost<W> work_list;																		///<工程列表

		protected:

			double time_out;

		public:

			MultiWorkProc()
			{
				time_out=5;
			}

			virtual ~MultiWorkProc()=default;

			void SetTimeOut(const double to)																///<设置超时时间
			{
				if(to<=0)time_out=0;
					else time_out=to;
			}

			virtual void Post(W *w)																			///<投递一个工作
			{
				if(!w)return;

				work_list.Post(w);
				work_list.ReleaseSem(1);
			}

			virtual void Post(W **w,int count)																///<投递一批工作
			{
				if(!w||count<=0)return;

				work_list.Post(w,count);
				work_list.ReleaseSem(count);
			}

		public:

			/**
			 * 开始执行工作函数
			 */
			virtual bool OnExecuteWork(const uint wt_index) override
			{
				W *obj=work_list.WaitSemReceive(time_out);

				if(!obj)
                    return(false);

				this->OnWork(wt_index,obj);

				return(true);
			}
		};//template<typename W> class MultiWorkProc:public WorkProc<W>

		/**
		 * 工作线程，用于真正处理事务
		 */
		template<typename W> class WorkThread:public Thread
		{
		protected:

			using WorkList=List<W *>;

			WorkProc<W> *work_proc;

			uint work_thread_index;

            ThreadMutex exit_lock;
            bool force_close;

		public:

			WorkThread(WorkProc<W> *wp)
			{
				work_proc=wp;
				work_thread_index=0;
				force_close=false;
			}

#ifndef _DEBUG
			virtual ~WorkThread()=default;
#else
            virtual ~WorkThread()
            {
                UTF8String thread_addr;

                GetAddress(thread_addr);

                LOG_INFO(U8_TEXT("WorkThread Destruct [")+thread_addr+U8_TEXT("]"));
            }
#endif//_DEBUG

            bool Start() override													  ///<开始运行当前线程
            {
                exit_lock.Lock();

                return Thread::Start();
            }

            bool IsExitDelete()const override{return false;}							///<返回在退出线程时，不删除本对象

			void SetWorkThreadIndex(const uint index)
			{
				work_thread_index=index;
			}

			void ExitWork(const bool fc)
			{
				force_close=fc;
                exit_lock.Unlock();
			}

			virtual bool Execute() override
			{
				if(!work_proc)
					RETURN_FALSE;

                bool result=work_proc->OnExecuteWork(work_thread_index);

                if(!exit_lock.TryLock())
                    return(true);

                if(!force_close)        //不是强退
                {
                    while(work_proc->OnExecuteWork(work_thread_index));     //把工作全部做完
                }

#ifdef _DEBUG
                {
                    UTF8String thread_addr;

                    GetAddress(thread_addr);

                    LOG_INFO(U8_TEXT("WorkThread Finish [")+thread_addr+U8_TEXT("]"));
                }
#endif//_DEBUG

                exit_lock.Unlock();
                return(false);
			}
		};//template<typename W> class WorkThread:public Thread

		/**
		 * 工作组<br>
		 * 用于管理一组的工作线程以及投递器<br>
		 * 注：可以一组工作线程共用一个投递器，也可以每个工作线程配一个投递器。工作组管理只为方便统一清理
		 */
		template<typename WP,typename WT> class WorkGroup
		{
			ObjectList<WP> wp_list;														///<投递器列表
			ObjectList<WT> wt_list;														///<工作线程列表

			bool run=false;

		public:

			virtual ~WorkGroup()
			{
				Close();
			}

			virtual bool Add(WP *wp)
			{
				if(!wp)return(false);

				wp_list.Add(wp);
				return(true);
			}

			virtual bool Add(WP **wp,const int count)
			{
				if(!wp)return(false);

				wp_list.Add(wp,count);
				return(true);
			}

			virtual bool Add(WT *wt)
			{
				if(!wt)return(false);

				int index=wt_list.Add(wt);
				wt->SetWorkThreadIndex(index);
				return(true);
			}

			virtual bool Add(WT **wt,const int count)
			{
				if(!wt)return(false);

				int index=wt_list.Add(wt,count);
				for(int i=0;i<count;i++)
				{
					(*wt)->SetWorkThreadIndex(index);
					++index;
					++wt;
				}

				return(true);
			}

			virtual bool Start()
			{
				int count=wt_list.GetCount();

				if(count<=0)
					RETURN_FALSE;

                WT **wt=wt_list.GetData();

				for(int i=0;i<count;i++)
					wt[i]->Start();

                run=true;
				return(true);
			}

			virtual void Close(bool force_close=false)
			{
                if(!run)return;

				int count=wt_list.GetCount();

                WT **wt=wt_list.GetData();

				for(int i=0;i<count;i++)
					wt[i]->ExitWork(force_close);

				for(int i=0;i<count;i++)
					wt[i]->Wait();

                run=false;
			}
		};//template<typename WP,typename WT> class WorkGroup
	}//namespace workflow
}//namespace hgl
#endif//HGL_WORKFLOW_INCLUDE
