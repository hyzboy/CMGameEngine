#include<hgl/thread/Semaphore.h>
#include<hgl/LogInfo.h>

#include<wchar.h>
#pragma warning(disable:4800)            // int -> bool 性能损失警告

namespace hgl
{
    /**
    * @param max_count 最大计数
    */
    Semaphore::Semaphore(int max_count)
    {
        ptr=CreateSemaphore(nullptr,0,max_count,nullptr);

        if(!ptr)
            LOG_ERROR(OS_TEXT("CreateSemaphore error,max_count=")+OSString(max_count));
    }

    Semaphore::~Semaphore()
    {
        CloseHandle(ptr);
    }

    /**
    * 发送信号
    * @param n 发送的信号数量
    * @return 是否释放成功
    */
    bool Semaphore::Post(int n)
    {
        if(n<=0)return(false);
        return(ReleaseSemaphore(ptr,n,nullptr));
    }

    /**
    * 尝试获取一个信号
    * @return 是否有取得信号
    */
    bool Semaphore::TryAcquire()
    {
        return(WaitForSingleObject(ptr,0)==WAIT_OBJECT_0);
    }

    /**
    * 等待并获取一个信号
    * @param time 等待的最长时间,使用0表示无限等待.(单位秒)
    * @return 是否等待到了,如果超过最长时间,仍未等到即为超时,返回false
    */
    bool Semaphore::Acquire(double time_out)
    {
        return(WaitForSingleObject(ptr,time_out>0?DWORD(time_out*1000):INFINITE)==WAIT_OBJECT_0);
    }
}//namespace hgl
