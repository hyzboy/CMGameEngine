#include<hgl/ExternalModule.h>
#include<hgl/platform/Platform.h>
#include<hgl/LogInfo.h>

#include<dlfcn.h>

namespace hgl
{
    ExternalModule *LoadExternalModule(const os_char *filename)
    {
		ExternalModulePointer fp=dlopen(filename,RTLD_LAZY);

		if(!fp)
		{
			LOG_ERROR(OS_TEXT("Load Module <")+OSString(filename)+OS_TEXT("> error! os info: ")+OSString(dlerror()));

			return(nullptr);
		}

		return(new ExternalModule(fp));
    }

	/**
	* 加载一个外部模块
	* @param name 模块文件名称
	* @return 是否加载成功
	*/
	bool ExternalModule::Load(const os_char *name)
	{
		Clear();

		fp=dlopen(name,RTLD_LAZY);

		if(!fp)
		{
			LOG_ERROR(OS_TEXT("Load Module <")+OSString(name)+OS_TEXT("> error! os info: ")+OSString(dlerror()));

			return(false);
		}

		return(true);
	}
}//namespace hgl
