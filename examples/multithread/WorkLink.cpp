#include<hgl/Console.h>
#include<hgl/thread/Thread.h>
#include<hgl/thread/ThreadMutex.h>
#include<hgl/type/Queue.h>

using namespace hgl;

/**
 * 该范例演示了由多个工作投递线程产生工作样本，要求多个工作处理线程进行处理的情况
 */

#define POST_THREAD_NUMBER		16		//投递线程数量
#define POST_ONE_MAX			128		//每次投递的数量
#define WORK_THREAD_NUMBER		16		//工作线程数量
#define WORK_QUEUE_BY_THREAD	16*16	//每工作线程对应的工作对数量

/**
 * 工作对象
 */
class WorkObject
{
	ThreadMutex lock;	//数值队列

	int value;			//当前值
	int count;			//处理计数器

public:

	WorkObject()
	{
		value=0;
		count=0;
	}

	void Push()
	{
		lock.Lock();
			++value;
		lock.Unlock();
	}

	bool Pop()
	{
		if(!lock.TryLock())
			return(false);

		--value;
		++count;

		lock.Unlock();
		return(true);
	}
};//class WorkObject

/**
 * 工作处理线程
 */
class WorkThread:public Thread
{
	WorkObject **wo_list;

public:

	WorkThread(WorkObject **wo)
	{
		wo_list=wo;
	}

	bool Execute()	//工作处理线程执行函数
	{
	}
};//class WorkThread

/**
 * 工作分发工具
 */
class PostTools
{
};//class PostTools

/**
 * 工作投递线程
 */
class PostThread:public Thread
{
	WorkObject **wo_list;

	PostTools *post;

public:

    PostThread(WorkObject **wo,PostTools *pt)
	{
		wo_list=wo;
		post=pt;
	}

	bool Execute()	//工作投递线程执行函数
	{
        const int num=rand()%POST_ONE_MAX;              //产生随机量的工作内容

		for(int i=0;i<num;i++)
		{
			int target=rand()%POST_THREAD_NUMBER;	    //随机一个投递目标

			wo_list[target]->Push();
		}

		double wait_time=float(rand()%1000)/1000.0f;		//随机等待一个时间，进行下一次投递

		Thread::Wait(wait_time);
		return(true);
	}
};//class PostThread

/**
 * 流程执行对象
 */
class TestObject:public ConsoleFlowObject
{
	WorkObject **wo_list;

	WorkThread *wt[WORK_THREAD_NUMBER];
	PostThread *pt[POST_THREAD_NUMBER];

public:

	TestObject()
	{
		wo_list=new WorkObject *[WORK_QUEUE_BY_THREAD*WORK_THREAD_NUMBER];

		for(int i=0;i<WORK_QUEUE_BY_THREAD*WORK_THREAD_NUMBER;i++)
			wo_list[i]=new WorkObject;

		for(int i=0;i<WORK_THREAD_NUMBER;i++)		//创建工作处理线程
		{
			wt[i]=new WorkThread(wo_list+(i*WORK_QUEUE_BY_THREAD));
			wt[i]->Start();
		}

		for(int i=0;i<POST_THREAD_NUMBER;i++)		//创建工作投递线程
		{
			pt[i]=new PostThread(wo_list);
			pt[i]->Start();
		}
	}

	void Update()
	{
	}
};//class TestObject

HGL_CONSOLE_APPLICATION("多线程工作汇集链",
						"WorkCabinet",
						new TestObject);
