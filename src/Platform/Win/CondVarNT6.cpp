#include<windows.h>
#include<hgl/thread/CondVar.h>
#include<hgl/thread/ThreadMutex.h>
#include<hgl/thread/RWLock.h>

#pragma warning(disable:4800)			// BOOL -> bool 性能损失警告
namespace hgl
{
	CondVar::CondVar()
	{
		cond_var = new CONDITION_VARIABLE;
		InitializeConditionVariable((CONDITION_VARIABLE *)cond_var);
	}

	CondVar::~CondVar()
	{
		delete (CONDITION_VARIABLE *)cond_var;
	}

	bool CondVar::Wait(ThreadMutex *tm, double time)
	{
		return SleepConditionVariableCS((CONDITION_VARIABLE *)cond_var,(CRITICAL_SECTION *)(tm->GetThreadMutex()),(DWORD)(time>0?time*1000:INFINITE));
	}

	bool CondVar::Wait(RWLock *lock, double time, bool read)
	{
		return SleepConditionVariableSRW((CONDITION_VARIABLE *)cond_var,(SRWLOCK *)(lock->GetRWLock()),(DWORD)(time>0?time*1000:INFINITE),read?CONDITION_VARIABLE_LOCKMODE_SHARED:0);
	}

	void CondVar::Signal()
	{
		WakeConditionVariable((CONDITION_VARIABLE *)cond_var);
	}

	void CondVar::Broadcast()
	{
		WakeAllConditionVariable((CONDITION_VARIABLE *)cond_var);
	}
}//namespace hgl
