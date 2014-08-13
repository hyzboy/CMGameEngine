#include<hgl/ExternalModule.h>
#include<hgl/LogInfo.h>

#if HGL_OS == HGL_OS_Windows
	#include<winbase.h>

	#define pi_get		GetProcAddress
	#define pi_close	FreeLibrary
#else
	#include<dlfcn.h>

	#define pi_get		dlsym
	#define pi_close	dlclose
#endif//HGL_OS == HGL_OS_Windows

namespace hgl
{
	ExternalModule *LoadExternalModule(const os_char *filename)
	{
		ExternalModulePointer fp;

#if HGL_OS == HGL_OS_Windows
		fp=LoadLibraryW(name);

		if(!fp)
		{
			uint dw=GetLastError();
			os_char *str=nullptr;

			FormatMessageW(	FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,nullptr,dw,
							MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),str,0,nullptr);

			LOG_ERROR(OS_TEXT("LoadModule <")+OSString(filename)+OS_TEXT("> error! os info: ")+OSString(dw)+OSString(',')+str);

			return(nullptr);
		}
#else
		fp=dlopen(filename,RTLD_LAZY);

		if(!fp)
		{
			LOG_ERROR(OS_TEXT("Load Module <")+OSString(filename)+OS_TEXT("> error! os info: ")+OSString(dlerror()));

			return(nullptr);
		}
#endif//HGL_OS == HGL_OS_Windows

		return(new ExternalModule(fp));
	}

	ExternalModule::ExternalModule()
	{
		fp=nullptr;
	}

	ExternalModule::ExternalModule(ExternalModulePointer emp)
	{
		fp=emp;
	}

	ExternalModule::~ExternalModule()
	{
		Clear();
	}

	/**
	* 加载一个外部模块
	* @param name 模块文件名称
	* @return 是否加载成功
	*/
	bool ExternalModule::Load(const os_char *name)
	{
		Clear();

#if HGL_OS == HGL_OS_Windows
		fp=LoadLibraryW(name);

		if(!fp)
		{
			uint dw=GetLastError();
			u16char *str=nullptr;

			FormatMessageW(	FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,nullptr,dw,
							MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),str,0,nullptr);

			LOG_ERROR(u"LoadModule <"+OSString(name)+u"> error! os info: "+UTF16String(dw)+UTF16String(L',')+str);

			return(false);
		}
#else
		fp=dlopen(name,RTLD_LAZY);

		if(!fp)
		{
			LOG_ERROR(OS_TEXT("Load Module <")+OSString(name)+OS_TEXT("> error! os info: ")+OSString(dlerror()));

			return(false);
		}
#endif//HGL_OS == HGL_OS_Windows

		return(true);
	}

	void ExternalModule::Clear()
	{
		if(fp)
		{
        	pi_close(fp);

			fp=nullptr;
		}
	}

	void *ExternalModule::GetFunc(const char *name)
	{
		if(fp)
		{
			void *func=pi_get(fp,name);

			if(!func)
				LOG_ERROR(u"don't find "+to_u16(name));

			return(func);
		}
		else
		{
			LOG_ERROR(u"Get func <"+to_u16(name)+u" need load module！");

			return(nullptr);
		}
	}

	void *ExternalModule::FindFunc(const char *name)
	{
		if(fp)
		{
			void *func=pi_get(fp,name);

			if(func)return(func);

			char _name[1024];

			_name[0]='_';
			strcpy(_name+1,name);

			func=pi_get(fp,_name);

			if(func)return(func);
		}

       	return(nullptr);
	}

	/**
	 * 读取函数列表
	 * @return 成功加载的函数数量
	 */
	int ExternalModule::Get(FuncLoad *flp)
	{
		if(!flp)return(-1);

		int count=0;

		while(flp->func_name[0])
		{
			(*(flp->func_pointer))=GetFunc(flp->func_name);

			if(*(flp->func_pointer))
				++count;

			++flp;
		}

		return(count);
	}
}//namespace hgl
