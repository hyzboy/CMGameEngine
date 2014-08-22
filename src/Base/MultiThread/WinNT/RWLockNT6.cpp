#include<windows.h>
#include<hgl/thread/RWLock.h>

#pragma warning(disable:4800)			// BOOL -> bool 性能损失警告
namespace hgl
{
	RWLock::RWLock()
	{
		lock = new SRWLOCK;

		InitializeSRWLock((SRWLOCK *)lock);
	}

	RWLock::~RWLock()
	{
		delete (SRWLOCK *)lock;
	}

	bool RWLock::TryReadLock()	{ return TryAcquireSRWLockShared((SRWLOCK *)lock); }
	bool RWLock::ReadLock()		{ AcquireSRWLockShared((SRWLOCK *)lock); return(true); }
	bool RWLock::ReadUnlock()	{ ReleaseSRWLockShared((SRWLOCK *)lock); return(true); }

	bool RWLock::TryWriteLock()	{ return TryAcquireSRWLockExclusive((SRWLOCK *)lock); }
	bool RWLock::WriteLock()	{ AcquireSRWLockExclusive((SRWLOCK *)lock); return(true); }
	bool RWLock::WriteUnlock()	{ ReleaseSRWLockExclusive((SRWLOCK *)lock); return(true); }
}//namespace hgl
