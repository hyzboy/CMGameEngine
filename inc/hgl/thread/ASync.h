#ifndef HGL_MULTI_THREAD_ASYNC_INCLUDE
#define HGL_MULTI_THREAD_ASYNC_INCLUDE

#include<hgl/thread/Thread.h>
namespace hgl
{
    /**
     * 异步工作线程
     */
    template<typename R,typename ...P> class ASyncThread:public Thread
    {
    protected:

        bool start;
        bool end;

        P...args;

        R result;

    public:

        R &GetResult(){return result;}

    public:

        ASyncThread(R (*thread_start_func)(P...),const R &init_result,P...args)
        {
        }
    };

    /**
     * 异步工作结果
     */
    template<typename R> class ASyncFuture
    {
    protected:

        ASyncThread *work_thread;

    public:

        ASyncFuture(ASyncThread *wt)
        {
            work_thread=wt;

        }

        virtual ~ASyncFuture();

        virtual Thread *GetThread(){return work_thread;}                                            ///<获取工作线程指针(如果你真的需求)
        virtual R &GetResult(){return work_thread->GetResult();}                                    ///<取得运行结果
    };//class ASyncFuture

    /**
     * 创建一个异步工作线程
     * @param thread_start_func 线程起始函数
     * @param init_result 结果的初始值
     * @param args 参数列表
     * @return 异步工作结果访问对象
     * @return nullptr 创建线程失败
     */
    template<typename R,typename ...P>
    ASyncFuture<R> *ASync(R (*thread_start_func)(P...),const R &init_result,P...args)            ///<创建一个异步工作线程
    {
        auto *work_thread=new ASyncThread<R,..P>;

        auto *future=new ASyncFuture<R>(work_thread);
    }
}//namespace hgl
#endif//HGL_MULTI_THREAD_ASYNC_INCLUDE
