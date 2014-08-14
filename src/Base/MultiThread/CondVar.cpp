#include<hgl/thread/CondVar.h>
#include<hgl/platform/Platform.h>

#if HGL_OS == HGL_OS_Windows
#include<windows.h>
#endif//HGL_OS == HGL_OS_Windows

namespace hgl
{
	CondVar *CreateCondVarAPR();		//Apache读写锁，跨平台
	CondVar *CreateCondVarPOSIX();		//UNIX/Linux平台

#if HGL_OS != HGL_OS_Windows			//非Windows平台
	CondVar *CreateCondVar()
	{
		return CreateCondVarPOSIX();
	}
#else

	void InitCondVarNT6();
	CondVar *CreateCondVarNT6();		//Windows Server 2008版条件变量，需要Windows Server 2008/Vista

	namespace
	{
		CondVar *(*CreateCondVarWin)()=nullptr;						//创建CondVar函数指针

		void CheckWindowsCondVar()									//检测Windows CondVar
		{
			OSVERSIONINFOEX osvi;

			memset(&osvi,0,sizeof(OSVERSIONINFOEX));

			osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);

			GetVersionEx((OSVERSIONINFO *)&osvi);

			if(osvi.dwMajorVersion>=6)
			{
				InitCondVarNT6();
				CreateCondVarWin=CreateCondVarNT6;					//使用Win 2008/Vista所提供的CondVar
			}
			else
				CreateCondVarWin=CreateCondVarAPR;					//使用APR模拟的CondVar
		}//void CheckWindowsSRWLock()
	}//namespace

	CondVar *CreateCondVar()
	{
		if(!CreateCondVarWin)
			CheckWindowsCondVar();

		return CreateCondVarWin();
	}//CondVar *CreateCondVar()
#endif//HGL_OS == HGL_OS_Windows
}//namespace hgl
