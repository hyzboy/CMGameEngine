#include<windows.h>
#include<hgl/thread/CondVar.h>
#include<hgl/thread/ThreadMutex.h>
#include<hgl/thread/RWLock.h>

#pragma warning(disable:4800)			// BOOL -> bool 性能损失警告
namespace hgl
{
	namespace
	{
		typedef VOID (WINAPI *InitCondVarFUNC		)(PCONDITION_VARIABLE ConditionVariable);
		typedef BOOL (WINAPI *WaitThreadMutexFUNC	)(PCONDITION_VARIABLE ConditionVariable,PCRITICAL_SECTION CriticalSection,DWORD dwMilliseconds);
		typedef BOOL (WINAPI *WaitRWLockFUNC		)(PCONDITION_VARIABLE ConditionVariable,PSRWLOCK SRWLock,DWORD dwMilliseconds,ULONG Flags);
		typedef VOID (WINAPI *SignalFUNC			)(PCONDITION_VARIABLE ConditionVariable);
		typedef VOID (WINAPI *BroadcastFUNC			)(PCONDITION_VARIABLE ConditionVariable);
	}

	class CondVarNT6:public CondVar
	{
		friend void InitCondVarNT6();

		static InitCondVarFUNC		InitCondVarNT6;
		static WaitThreadMutexFUNC	WaitThreadMutexNT6;
		static WaitRWLockFUNC		WaitRWLockNT6;
		static SignalFUNC			SignalNT6;
		static BroadcastFUNC		BroadcastNT6;

	private:

		CONDITION_VARIABLE cond;

	public:

		CondVarNT6()
		{
			//InitializeConditionVariable
			InitCondVarNT6(&cond);
		}

		~CondVarNT6()HGL_DEFAULT_MEMFUNC;

		bool Wait(ThreadMutex *tm,double time)
		{
			return //SleepConditionVariableCS
				WaitThreadMutexNT6(&cond,(CRITICAL_SECTION *)(tm->GetThreadMutex()),(DWORD)(time>0?time*1000:INFINITE));
		}

		bool Wait(RWLock *lock,double time,bool read)
		{
			return //SleepConditionVariableSRW
				WaitRWLockNT6(&cond,(SRWLOCK *)(lock->GetRWLock()),(DWORD)(time>0?time*1000:INFINITE),read?CONDITION_VARIABLE_LOCKMODE_SHARED:0);
		}

		void Signal()
		{
			//WakeConditionVariable
			SignalNT6(&cond);
		}

		void Broadcast()
		{
			//WakeAllConditionVariable
			BroadcastNT6(&cond);
		}
	};//class CondVarAPR

	InitCondVarFUNC		CondVarNT6::InitCondVarNT6		=nullptr;
	WaitThreadMutexFUNC	CondVarNT6::WaitThreadMutexNT6	=nullptr;
	WaitRWLockFUNC		CondVarNT6::WaitRWLockNT6		=nullptr;
	SignalFUNC			CondVarNT6::SignalNT6			=nullptr;
	BroadcastFUNC		CondVarNT6::BroadcastNT6		=nullptr;

	void InitCondVarNT6()
	{
		HMODULE module=LoadLibrary(u"Kernel32.dll");

		CondVarNT6::InitCondVarNT6		=(InitCondVarFUNC		)GetProcAddress(module,"InitializeConditionVariable");
		CondVarNT6::WaitThreadMutexNT6	=(WaitThreadMutexFUNC	)GetProcAddress(module,"SleepConditionVariableCS");
		CondVarNT6::WaitRWLockNT6		=(WaitRWLockFUNC		)GetProcAddress(module,"SleepConditionVariableSRW");
		CondVarNT6::SignalNT6			=(SignalFUNC			)GetProcAddress(module,"WakeConditionVariable");
		CondVarNT6::BroadcastNT6		=(BroadcastFUNC			)GetProcAddress(module,"WakeAllConditionVariable");

		FreeLibrary(module);
	}//void InitCondVarNT6()
}//namespace hgl
