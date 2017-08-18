#ifndef HGL_WORKFLOW_INCLUDE
#define HGL_WORKFLOW_INCLUDE

#include<hgl/thread/Thread.h>
#include<hgl/thread/SwapData.h>
#include<hgl/type/List.h>
namespace hgl
{
	/**
	 * 工作流名字空间<br>
	 * 工作流是一种对工作的多线安排管理机制，它适用于按业务分配多线程的工作环境<br>
	 * 开发者需要为每一种工作指定一定的线程数量，但每一种工作确只有一个工作分配入口和分发出口。<br>
	 * 由其它程序提交工作任务到入口，开发者可以自行重载分配入口的分配函数。
     *
     * 使用方法：
     *
	 *	group=new WorkGroup;			//创建group
     *
	 *	WorkPost *wp[10];
     *
	 *	for(int i=0;i<MAX_THREADS;i++)
	 *	{
	 *		wp=new WorkPost();			//创建事件投递器
	 *		wt=new WorkThread(wp);		//创建工作线程
     *
	 *		group->Add(wp);				//添加投递器到group
	 *		group->Add(wt);				//添加工作线程到group
	 *	}
     *
	 *	group->Start();					//启动group，开启所有工作线程
     *
	 *	for(int i=0;i<0xffff;i++)
	 *	{
	 *		int index=rand()%10;		//随机一个线程
     *
	 *		for(int j=0;j<rand()%10;j++)
	 *		{
	 *			wp[index]->Post(new Work);			//投递一个工作
	 *		}
     *
	 *		wp[index]->ToWork();
	 *	}
     *
	 *	group->Close();					//关闭group,关闭所有工作线程
	 */
	namespace workflow
	{
		/**
		 * 入口分配器<br>
		 * Post的工作内容是将任务分配给所对应的WorkThread
		 */
		template<typename W> class WorkPost
		{
		protected:

			using WorkList=List<W *>;

			SemSwapData<WorkList> work_list;																///<工程列表

		public:

			virtual ~WorkPost()
			{
			}

			virtual void Post(W *w)																			///<投递一个工作
			{
				WorkList &wl=work_list.GetBack();
					wl.Add(w);
				work_list.ReleaseBack();
			}

			virtual void Post(W **w,int count)																///<投递一批工作
			{
				WorkList &wl=work_list.GetBack();
					wl.Add(w,count);
				work_list.ReleaseBack();
			}

			virtual void ToWork()																			///<将堆积的工作列表发送给工作线程
			{
				work_list.ReleaseSem(1);
			}

		public:

			WorkList *WaitWorkList(const double time_out=5)
			{
				if(work_list.WaitSemSwap(time_out))
					return &(work_list.GetFront());

				return(nullptr);
			}
		};//template<typename W> class WorkPost

		/**
		 * 工作线程，用于真正处理事务
		 */
		template<typename W> class WorkThread:public Thread
		{
		protected:

			using WorkList=List<W *>;

			WorkPost<W> *work_post;
			WorkList *wl;

			atom_bool exit_work;																///<退出标记

		public:

			WorkThread(WorkPost<W> *wp)
			{
				work_post=wp;
				wl=nullptr;
				exit_work=false;
			}

			virtual ~WorkThread()
			{
			}

            virtual bool IsExitDelete()const override{return false;}								///<返回在退出线程时，不删除本对象

			virtual void ProcWork(W *obj)=0;														///<直接处理工作的纯虚函数，需使用者重载实现

			virtual void ExitWork()
			{
				exit_work=true;
			}

			virtual bool Execute() override
			{
				if(!work_post)
					RETURN_FALSE;

				wl=work_post->WaitWorkList();

				if(!wl)
					return(!exit_work);

				W **p=wl->GetData();

				for(int i=0;i<wl->GetCount();i++)
				{
					ProcWork(*p);
					++p;
				}

				wl->ClearData();

				return(!exit_work);
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

		public:

			WorkGroup()
			{
			}

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

				wt_list.Add(wt);
				return(true);
			}

			virtual bool Add(WT *wt,const int count)
			{
				if(!wt)return(false);

				wt_list.Add(wt,count);
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

				return(true);
			}

			virtual void Close()
			{
				int count=wt_list.GetCount();

                WT **wt=wt_list.GetData();

				for(int i=0;i<count;i++)
					wt[i]->ExitWork();

				for(int i=0;i<count;i++)
					wt[i]->Wait();
			}
		};//template<typename WP,typename WT> class WorkGroup
	}//namespace workflow
}//namespace hgl
#endif//HGL_WORKFLOW_INCLUDE
