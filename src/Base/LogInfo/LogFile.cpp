#include<hgl/Logger.h>
#include<hgl/File.h>
#include<hgl/Other.h>
#include<hgl/thread/ThreadMutex.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/io/TextOutputStream.h>

#if HGL_OS == HGL_OS_Windows
	#include<windows.h>
	#include<shlobj.h>
#else
	#include<pwd.h>
	#include<unistd.h>
#endif//HGL_OS == HGL_OS_Windows

namespace hgl
{
	namespace logger
	{
		class LogFile:public Logger
		{
			ThreadMutex lock;

			io::FileOutputStream 	fos;
			io::UTF16LETextOutputStream *tos;

		public:

			LogFile(LogLevel ll):Logger(ll)
			{
				tos=nullptr;
			}

			~LogFile()
			{
				SAFE_CLEAR(tos);
			}

			bool Create(const OSString &project_code)
			{
				os_char filename[HGL_MAX_PATH];
				os_char fn[HGL_MAX_PATH];
				os_char num[16]={'.',0};

			#if HGL_OS == HGL_OS_Windows
				SHGetFolderPathW(nullptr,CSIDL_LOCAL_APPDATA,nullptr,0,fn);
			#else
				struct passwd pwd;
				struct passwd *result;
				char *buf;
				size_t bufsize=sysconf(_SC_GETPW_R_SIZE_MAX);

				if(bufsize==-1)
					bufsize=16384;

				buf=(char *)malloc(bufsize);

				getpwuid_r(getuid(),&pwd,buf,bufsize,&result);

				strcpy(fn,HGL_MAX_PATH,pwd.pw_dir);

				free(buf);
			#endif//HGL_OS == HGL_OS_Windows

				strcat(fn,HGL_MAX_PATH,HGL_DIRECTORY_SEPARATOR);
				strcat(fn,HGL_MAX_PATH,OS_TEXT(".cmgdk"),6);
				MakePath(fn);

				strcat(fn,HGL_MAX_PATH,HGL_DIRECTORY_SEPARATOR);

#if HGL_OS == HGL_OS_Windows
				strcat(fn, HGL_MAX_PATH,project_code);
#else
				strcat(fn,HGL_MAX_PATH,project_code);
#endif//HGL_OS == HGL_OS_Windows

				for(uint i=0;i<=0xFFFF;i++)
				{
					strcpy(filename,HGL_MAX_PATH,fn);

					if(i)
					{
						utos(num+1,14,i);
						strcat(filename,HGL_MAX_PATH,num,sizeof(num));
					}

					strcat(filename,HGL_MAX_PATH,OS_TEXT(".Loginfo"),8);

					if(fos.Create(filename))//创建成功
					{
						tos=new io::UTF16LETextOutputStream(&fos);
						tos->WriteBOM();

						return(true);
					}
				}

				return(false);
			}

			void Close()
			{
				SAFE_CLEAR(tos);
				fos.Close();
			}

			void Write(const u16char *str,int size)
			{
				if(tos&&str&&*str&&size>0)
				{
//					lock.Lock();
					tos->WriteLine(str,size);
//					lock.Unlock();
				}
			}

			void Write(const char *str,int size)
			{
				if(tos&&str&&*str&&size>0)
				{
//					lock.Lock();
					tos->WriteLine(str,size);
//					lock.Unlock();
				}
			}
		};//class LogFile

		Logger *CreateLoggerFile(const OSString &pc,LogLevel ll)
		{
			if(ll<llError)
				return(nullptr);

			LogFile *lf=new LogFile(ll);

			if(lf->Create(pc))
				return lf;

			delete lf;
			return(nullptr);
		}
	}//namespace logger
}//namespace hgl
