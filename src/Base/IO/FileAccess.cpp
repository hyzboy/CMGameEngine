#include<hgl/io/FileAccess.h>
#include<hgl/LogInfo.h>
#if HGL_OS == HGL_OS_Windows
	#include<io.h>
	#include<share.h>
#else
	#include<unistd.h>
	#include<stdlib.h>
#endif//HGL_OS == HGL_OS_Windows
#include<fcntl.h>
namespace hgl
{
	namespace io
	{
		FileAccess::FileAccess()
		{
			fp=-1;
			mode=fomNone;
		}

		FileAccess::~FileAccess()
		{
			Close();
		}

		bool FileAccess::CreateTemp()
		{
#if HGL_OS == HGL_OS_Windows
			const uint buf_size=HGL_MAX_PATH;

			char16_t PathBuffer[buf_size];
			char16_t TempName[buf_size];

			GetTempPathW(buf_size,PathBuffer);

			GetTempFileNameW(PathBuffer,u"NEW",0,TempName);

			return Open(TempName,fomCreate);
#else
			char template_filename[128]="/tmp/cm/XXXXXX";

			fp=mkstemps(template_filename,128);

			if(fp==-1)
				return(false);

			filename=template_filename;
			mode=fomCreate;

			return(true);
#endif//HGL_OS == HGL_OS_Windows
		}

		bool FileAccess::Open(const OSString &fn,FileOpenMode fom)
		{
			Close();

#if HGL_OS == HGL_OS_Windows
			errno_t result;

			if(fom==fomCreate		)result=_wsopen_s(&fp,fn,_O_BINARY|_O_WRONLY|_O_CREAT			,_SH_DENYNO,S_IREAD|_S_IWRITE);else
			if(fom==fomCreateTrunc 	)result=_wsopen_s(&fp,fn,_O_BINARY|_O_WRONLY|_O_CREAT|_O_TRUNC	,_SH_DENYNO,S_IREAD|_S_IWRITE);else
			if(fom==fomOnlyRead		)result=_wsopen_s(&fp,fn,_O_BINARY|_O_RDONLY					,_SH_DENYNO,S_IREAD|_S_IWRITE);else
			if(fom==fomOnlyWrite	)result=_wsopen_s(&fp,fn,_O_BINARY|_O_WRONLY					,_SH_DENYNO,S_IREAD|_S_IWRITE);else
			if(fom==fomReadWrite	)result=_wsopen_s(&fp,fn,_O_BINARY|_O_RDWR						,_SH_DENYNO,S_IREAD|_S_IWRITE);else
			if(fom==fomAppend		)result=_wsopen_s(&fp,fn,_O_BINARY|_O_APPEND					,_SH_DENYNO,S_IREAD|_S_IWRITE);else
#else
			if(fom==fomCreate		)fp=open64(fn.c_str(),O_WRONLY|O_CREAT,			S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);else
			if(fom==fomCreateTrunc	)fp=open64(fn.c_str(),O_WRONLY|O_CREAT|O_TRUNC,	S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);else
			if(fom==fomOnlyRead		)fp=open64(fn.c_str(),O_RDONLY	);else
			if(fom==fomOnlyWrite	)fp=open64(fn.c_str(),O_WRONLY	);else
			if(fom==fomReadWrite	)fp=open64(fn.c_str(),O_RDWR	);else
			if(fom==fomAppend		)fp=open64(fn.c_str(),O_APPEND	);else
#endif//HGL_OS == HGL_OS_Windows
			{
				LOG_ERROR(OS_TEXT("Error FileOpenMode,filename: ")+fn);
				return(false);
			}

			if(fp==-1)
			{
				LOG_ERROR(OS_TEXT("open file error,filename: ")+fn);
				return(false);
			}

			filename=fn;
			mode=fom;
			return(true);
		}

		void FileAccess::Close()
		{
			if(fp==-1)return;

#if HGL_OS == HGL_OS_Windows
			_close(fp);
#else
			close(fp);
#endif//HGL_OS == HGL_OS_Windows
			fp=-1;
			mode=fomNone;
		}

		void FileAccess::CloseRead()
		{
			if(fp==-1)return;

			if(!CanWrite())
				Close();

			mode=fomOnlyWrite;
		}

		void FileAccess::CloseWrite()
		{
			if(fp==-1)return;

			if(!CanRead())
				Close();

			mode=fomOnlyRead;
		}

		bool FileAccess::CanRead() const
		{
			if(fp==-1)return(false);

			if(mode==fomOnlyWrite)return(false);

			return true;
		}

		bool FileAccess::CanWrite() const
		{
			if(fp==-1)return(false);

			if(mode==fomOnlyRead)return(false);

			return(true);
		}

		bool FileAccess::CanSeek() const
		{
			if(fp==-1)return(false);
			if(mode<fomCreate||mode>=fomAppend)return(false);

			return(true);
		}

		int64 FileAccess::Seek(int64 offset,SeekOrigin orign)
		{
			if(!CanSeek())return(-1);

			return lseek64(fp,offset,orign);
		}

		int64 FileAccess::Tell()const
		{
			if(fp==-1)return(-1);

			return tell64(fp);
		}

		int64 FileAccess::GetSize()
		{
			if(!CanSize())return(-1);

			fstat64(fp,&file_state);
			return(file_state.st_size);
		}

		int64 FileAccess::AvailableRead()
		{
			if(fp==-1)return(-1);

			if(mode!=fomOnlyRead
			 &&mode!=fomReadWrite)
				return(-1);

			return GetSize()-Tell();
		}

		bool FileAccess::Restart()
		{
			if(!CanRestart())return(false);

			return(Seek(0,soBegin)==0);
		}

		int64 FileAccess::Read(void *buf,int64 size)
		{
			if(!CanRead())return(-1);

			return read64(fp,buf,size);
		}

		int64 FileAccess::Peek(void *buf,int64 size)
		{
			if(!CanPeek())return(-1);

			int result=read64(fp,buf,size);

			if(result>0)
				lseek64(fp,-result,soCurrent);		//回移这个长度

			return result;
		}

		int64 FileAccess::Write(const void *buf,int64 size)
		{
			if(!CanWrite())return(-1);

			return write64(fp,buf,size);
		}

		int64 FileAccess::Read(int64 offset,void *buf,int64 size)
		{
			if(!CanRead())return(-1);

#if HGL_OS == HGL_OS_Windows
			if(_lseeki64(fp,offset,soBegin)==offset)
				return _read(fp,buf,size);
			else
				return -1;
#else
			return pread64(fp,buf,size,offset);
#endif//HGL_OS == HGL_OS_Windows
		}

		int64 FileAccess::Write(int64 offset,const void *buf,int64 size)
		{
			if(!CanWrite())return(-1);

#if HGL_OS == HGL_OS_Windows
			if(_lseeki64(fp,offset,soBegin)==offset)
				return _write(fp,buf,size);
			else
				return -1;
#else
			return pwrite64(fp,buf,size,offset);
#endif//HGL_OS == HGL_OS_Windows
		}
	}//namespace io
}//namespace hgl
