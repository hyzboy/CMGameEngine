#ifndef HGL_MULTI_THREAD_CONDITION_VARIABLE_INCLUDE
#define HGL_MULTI_THREAD_CONDITION_VARIABLE_INCLUDE

#include<hgl/platform/Platform.h>
namespace hgl
{
    class ThreadMutex;
    class RWLock;

    /**
    * 条件变量
    */
    class CondVar
    {
        void *cond_var;

    public:

        CondVar();
        virtual ~CondVar();

        bool Wait(ThreadMutex *,double time=0);                                                     ///<释放指定ThreadMutex，在条件符合后重新锁定并结束等待,0表示永久

#if HGL_OS == HGL_OS_Windows
        bool Wait(RWLock *,double time=0,bool read=false);                                          ///<释放指定RWLock，在条件符合后重新锁定并结束等待。（注：此函数仅在Windows 2008/Vista及更新版本中支持，Linux/BSD版不支持）
#endif//HGL_OS == HGL_OS_Windows

        void Signal();                                                                              ///<发送一个信号,让一个等待的线程解锁
        void Broadcast();                                                                           ///<广播一个信号,让所有等待的线程都解锁
    };//class CondVar
}//namespace hgl
#endif//HGL_MULTI_THREAD_CONDITION_VARIABLE_INCLUDE
