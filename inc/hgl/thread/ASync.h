#ifndef HGL_MULTI_THREAD_ASYNC_INCLUDE
#define HGL_MULTI_THREAD_ASYNC_INCLUDE

#include<hgl/thread/Thread.h>
namespace hgl
{
    未完成，不可用


    /**
     * 异步工作线程
     */
    class ASyncThread:public Thread
    {
    public:

        ASyncThread(R (*work_start_func)(const P &))
        {
        }
    };//class ASyncThread:public Thread

    /**
     * 异步工作线程
     */
    template<typename R,typename P> class ASyncThread:public Thread
    {
    public:

        P arg;
        R result;

    };//class ASyncThread

    template<typename P> class ASyncThread<void,P>:public Thread
    {
    public:

        P arg;

    public:

        AsyncThread(void (*work_start_func)(const P &))
        {
        }
    };//class ASyncThread<void,P>

    template<typename R> class ASyncThread<R,void>:public Thread
    {
    public:

        R result;

    public:

        ASyncThread(R (*work_start_func)(void))
        {
        }
    };//class ASyncThread<R,void>

    template<> class ASyncThread<void,void>:public Thread
    {
    public:

        ASyncThread(void (*work_start_func)())
        {
            Thread::Start();
        }
    };//class ASyncThread<void,void>

    /**
     * 异步工作结果
     */
    template<typename R> class ASyncResult
    {
    protected:

        ASyncThread *work_thread;

    public:

        ASyncResult(ASyncThread *wt)
        {
            work_thread=wt;
        }

        virtual ~ASyncResult();

        virtual Thread *GetThread(){return work_thread;}                                            ///<获取工作线程指针(如果你真的需求)
        virtual R &GetResult(){return work_thread->GetResult();}                                    ///<取得运行结果
    };//class ASyncResult

    /**
     * 创建一个异步工作
     * @param work_start_func 工作起始函数
     * @param init_result 结果的初始值
     * @param arg 参数
     * @return 异步工作结果访问对象
     * @return nullptr 创建线程失败
     */
    template<typename R,typename P>
    inline ASyncResult<R> *ASync(R (*work_start_func)(const P &),const R &init_result,const P &arg)      ///<创建一个异步工作
    {
        ASyncThread<R,P> *work_thread=new ASyncThread<R,P>(work_start_func);

        work_thread->result =init_result;
        work_thread->arg    =arg;

        ASyncResult<R> *future=new ASyncResult<R>(work_thread);
    }

    template<typename P>
    inline void ASyncNoReturn(void (*work_start_func)(void),const P &arg)
    {
    }

    template<typename R>
    inline ASyncResult<R> *ASyncNoParams(R (*work_start_func)(void),const R &init_result)
    {
    }

    inline ASyncResult<void> *ASync(void (*work_start_func)())
    {
        return(new ASyncResult<void>(new ASyncThread<void,void>(work_start_func)));
    }
}//namespace hgl
#endif//HGL_MULTI_THREAD_ASYNC_INCLUDE
