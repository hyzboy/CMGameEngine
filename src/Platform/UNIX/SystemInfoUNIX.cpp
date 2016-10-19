#include<hgl/platform/SystemInfo.h>
#include<hgl/Other.h>
#include<hgl/File.h>
#include<pwd.h>
#include<unistd.h>

namespace hgl
{
	bool GetCMGDKPath(OSString &cmgdk_path)
	{
		const char path_list[][18]=
		{
			"/usr/share/cmgdk",
			"/usr/local/cmgdk"
		};

		char home_path[HGL_MAX_PATH];

		struct passwd *pwd=getpwuid(getuid());

		memset(home_path,0,sizeof(home_path));

		strcpy(home_path,HGL_MAX_PATH,pwd->pw_dir);
		strcat(home_path,HGL_MAX_PATH,"/cmgdk",6);

		if(IsDirectory(home_path))
		{
			cmgdk_path=home_path;
			return(true);
		}

		for(int i=0;i<sizeof(path_list)/sizeof(path_list[0]);i++)
		{
			if(IsDirectory(path_list[i]))
			{
				cmgdk_path=path_list[i];
				return(true);
			}
		}

		return(false);
	}

	void GetOSPath(CMGDKPATH &cp)
	{
		cp.os			=OS_TEXT("/");
		cp.osfont		=OS_TEXT("/usr/share/fonts");

#if HGL_CPU == HGL_CPU_X86_64
		cp.library		=OS_TEXT("/usr/lib64");
#else
		cp.library		=OS_TEXT("/usr/lib");
#endif//HGL_CPU == HGL_CPU_X86_64

		cp.common_data	=OS_TEXT("/usr/share");
		cp.local_data	=OS_TEXT("/usr/local");

		cp.temp			=OS_TEXT("/tmp");

		{
			struct passwd pwd;
			struct passwd *result;
			char *buf;
			size_t bufsize=sysconf(_SC_GETPW_R_SIZE_MAX);

			if(bufsize==-1)
				bufsize=16384;

			buf=(char *)malloc(bufsize);

			getpwuid_r(getuid(),&pwd,buf,bufsize,&result);

			cp.mydata		=pwd.pw_dir;

			free(buf);
		}

		cp.myprogram	=cp.mydata+OS_TEXT("/bin");
		cp.mydesktop	=cp.mydata+OS_TEXT("/Desktop");
	}
}//namespace hgl
