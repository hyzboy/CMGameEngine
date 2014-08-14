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
	void RWLock::ReadLock()		{ AcquireSRWLockShared((SRWLOCK *)lock); }
	void RWLock::ReadUnlock()	{ ReleaseSRWLockShared((SRWLOCK *)lock); }

	bool RWLock::TryWriteLock()	{ return TryAcquireSRWLockExclusive((SRWLOCK *)lock); }
	void RWLock::WriteLock()	{ AcquireSRWLockExclusive((SRWLOCK *)lock); }
	void RWLock::WriteUnlock()	{ ReleaseSRWLockExclusive((SRWLOCK *)lock); }
}//namespace hgl
