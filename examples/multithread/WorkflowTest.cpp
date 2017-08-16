#include<hgl/Console.h>
#include<hgl/Workflow.h>

using namespace hgl;
using namespace hgl::workflow;

/**
 * 该范例演示了由多个工作投递线程产生工作样本，要求多个工作处理线程进行处理的情况
 */

#define POST_THREAD_NUMBER		16		//投递线程数量
#define POST_ONE_MAX			128		//每次投递的数量
#define WORK_THREAD_NUMBER		16		//工作线程数量
#define WORK_QUEUE_BY_THREAD	16*16	//每工作线程对应的工作对数量

/**
* 工作内容
*/
struct MyWork
{
	int value;
};

using MyWorkPost=WorkPost<MyWork>;				//重定义工作投递器

class MyWorkThread:public WorkThread<MyWork>	//实现工作线程类
{
public:

	using WorkThread<MyWork>::WorkThread;

	void ProcWork(MyWork *obj) override			//实现具体工具处理函数
	{
		std::cout<<""<<obj->value<<std::endl;
		delete obj;
	}
};//class MyWorkThread

HGL_CONSOLE_MAIN_FUNC()
{
	WorkGroup<MyWorkPost,MyWorkThread> group;

	MyWorkPost *wp[POST_THREAD_NUMBER];
	MyWorkThread *wt[WORK_THREAD_NUMBER];


}