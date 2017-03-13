#include<hgl/proc/ProcMutex.h>
#include<windows.h>

namespace hgl
{
    ProcMutex::ProcMutex()
    {
        lock = nullptr;
    }

    bool ProcMutex::Create(const os_char *name)
    {
        if (lock != nullptr)return(false);

        lock = CreateMutexW(NULL, FALSE, name);

        if(!lock)
            return(true);

        lock = nullptr;
        return(false);
    }

    void ProcMutex::Clear()
    {
        if (lock == nullptr)return;

        CloseHandle(lock);

        lock = nullptr;
    }

    bool ProcMutex::Lock()
    {
        if (lock == nullptr)return(false);

        DWORD rv = WaitForSingleObject(lock, INFINITE);

        if (rv == WAIT_OBJECT_0 || rv == WAIT_ABANDONED)
            return(true);

        return(false);
    }

    bool ProcMutex::TryLock()
    {
        if (lock == nullptr)return(false);

        DWORD rv = WaitForSingleObject(lock, 0);

        if (rv == WAIT_OBJECT_0 || rv == WAIT_ABANDONED)
            return(true);

        return(false);
    }

    bool ProcMutex::Unlock()
    {
        if (lock == nullptr)return(false);

        return(!ReleaseMutex(lock));
    }
}//namespace hgl
