#include<hgl/thread/ThreadMutex.h>

#include<wchar.h>
#include<windows.h>

#pragma warning(disable:4800)            // BOOL -> bool 性能损失警告
namespace hgl
{
    ThreadMutex::ThreadMutex()
    {
        InitializeCriticalSection(&ptr);
    }

    ThreadMutex::~ThreadMutex()
    {
        Unlock();

        DeleteCriticalSection(&ptr);
    }

    /**
    * 取得控制权
    * 如果对象处于排斥状态，则等待
    */
    void ThreadMutex::Lock()
    {
        EnterCriticalSection(&ptr);
    }

    /**
    * 尝试取得控制权
    * @return 是否成功取得控制权
    */
    bool ThreadMutex::TryLock()
    {
        return(TryEnterCriticalSection(&ptr));
    }

    /**
    * 等待并取得控制权
    * @param time 等待的最大时间,时间为0表示尝试
    * @return 是否取得控制权
    */
    bool ThreadMutex::WaitLock(double time)
    {
        return(!WaitForSingleObject(&ptr,time*1000));
    }

    /**
    * 放弃控制权
    */
    void ThreadMutex::Unlock()
    {
        LeaveCriticalSection(&ptr);
    }
}//namespace hgl
