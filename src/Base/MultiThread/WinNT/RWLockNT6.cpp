#include<windows.h>
#include<hgl/thread/RWLock.h>

#pragma warning(disable:4800)			// BOOL -> bool 性能损失警告
namespace hgl
{
	RWLock *CreateRWLockAPR();		//Apache读写锁，跨平台

	//Slim Reader/Writer Lock 必须Windows 7或2008 R2
	//如不使用TryAcquire则在Vista/2008上也可以使用

	namespace
	{
		typedef VOID	(WINAPI *InitRWLockFUNC		)(PSRWLOCK SRWLock);
		typedef VOID	(WINAPI *WriteUnlockFUNC	)(PSRWLOCK SRWLock);
		typedef VOID	(WINAPI *ReadUnlockFUNC		)(PSRWLOCK SRWLock);
		typedef VOID	(WINAPI *WriteLockFUNC		)(PSRWLOCK SRWLock);
		typedef VOID	(WINAPI *ReadLockFUNC		)(PSRWLOCK SRWLock);
		typedef BOOLEAN (WINAPI *TryWriteLockFUNC	)(PSRWLOCK SRWLock);
		typedef BOOLEAN (WINAPI *TryReadLockFUNC	)(PSRWLOCK SRWLock);
	}

	class RWLockNT6:public RWLock
	{
		friend void InitRWLockNT6();

		static InitRWLockFUNC	InitRWLockNT6;
		static WriteUnlockFUNC	WriteUnlockNT6;
		static ReadUnlockFUNC	ReadUnlockNT6;
		static WriteLockFUNC	WriteLockNT6;
		static ReadLockFUNC		ReadLockNT6;
		static TryWriteLockFUNC	TryWriteLockNT6;
		static TryReadLockFUNC	TryReadLockNT6;

	private:

		SRWLOCK lock;

	public:

		RWLockNT6()
		{
			InitRWLockNT6(&lock);
		}

		~RWLockNT6()
		{
		}

		void *GetRWLock()	{return &lock;}

		bool TryReadLock()	{return TryReadLockNT6(&lock);}
		void ReadLock()		{ReadUnlockNT6(&lock);}
		void ReadUnlock()	{ReadUnlockNT6(&lock);}

		bool TryWriteLock()	{return TryWriteLockNT6(&lock);}
		void WriteLock()	{WriteLockNT6(&lock);}
		void WriteUnlock()	{WriteUnlockNT6(&lock);}
	};//class RWLockNT6

	InitRWLockFUNC		RWLockNT6::InitRWLockNT6	=nullptr;
	WriteUnlockFUNC		RWLockNT6::WriteUnlockNT6	=nullptr;
	ReadUnlockFUNC		RWLockNT6::ReadUnlockNT6	=nullptr;
	WriteLockFUNC		RWLockNT6::WriteLockNT6		=nullptr;
	ReadLockFUNC		RWLockNT6::ReadLockNT6		=nullptr;
	TryWriteLockFUNC	RWLockNT6::TryWriteLockNT6	=nullptr;
	TryReadLockFUNC		RWLockNT6::TryReadLockNT6	=nullptr;

	void InitRWLockNT6()
	{
		HMODULE module=LoadLibrary(u"Kernel32.dll");

		RWLockNT6::InitRWLockNT6	=(InitRWLockFUNC	)GetProcAddress(module,"InitializeSRWLock");
		RWLockNT6::WriteUnlockNT6	=(WriteUnlockFUNC	)GetProcAddress(module,"ReleaseSRWLockExclusive");
		RWLockNT6::ReadUnlockNT6	=(ReadUnlockFUNC	)GetProcAddress(module,"ReleaseSRWLockShared");
		RWLockNT6::WriteLockNT6		=(WriteLockFUNC		)GetProcAddress(module,"AcquireSRWLockExclusive");
		RWLockNT6::ReadLockNT6		=(ReadLockFUNC		)GetProcAddress(module,"AcquireSRWLockShared");
		RWLockNT6::TryWriteLockNT6	=(TryWriteLockFUNC	)GetProcAddress(module,"TryAcquireSRWLockExclusive");
		RWLockNT6::TryReadLockNT6	=(TryReadLockFUNC	)GetProcAddress(module,"TryAcquireSRWLockShared");

		FreeLibrary(module);
	}

	RWLock *CreateRWLockNT6()
	{
		return(new RWLockNT6());
	}

	namespace
	{
		RWLock *(*CreateRWLockWin)()=nullptr;							//创建RWLock函数指针

		void CheckWindowsSRWLock()									//检测Windows SRWLock
		{
			OSVERSIONINFOEX osvi;

			memset(&osvi,0,sizeof(OSVERSIONINFOEX));

			osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);

			GetVersionEx((OSVERSIONINFO *)&osvi);

			if(osvi.dwMajorVersion>6
			||(osvi.dwMajorVersion==6&&osvi.dwMinorVersion>=1))
			{
				InitRWLockNT6();
				CreateRWLockWin=CreateRWLockNT6;					//使用Win7/2008R2所提供的RWLock
			}
			else
				CreateRWLockWin=CreateRWLockAPR;					//使用APR模拟的RWLock
		}//void CheckWindowsSRWLock()
	}//namespace

	RWLock *CreateRWLock()
	{
		if(!CreateRWLockWin)
			CheckWindowsSRWLock();

		return CreateRWLockWin();
	}//RWLock *CreateRWLock()
}//namespace hgl
