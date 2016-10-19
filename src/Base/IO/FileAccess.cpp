#include<hgl/io/FileAccess.h>
#include<hgl/LogInfo.h>
#include<unistd.h>
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

		int OpenFile(const os_char *fn,FileOpenMode fom);

		bool FileAccess::Open(const OSString &fn,FileOpenMode fom)
		{
			Close();

            if(fn.IsEmpty())
            {
                LOG_ERROR(OS_TEXT("Error,filename is NULL"));
                return(false);
            }

            fp=OpenFile(fn.c_str(),fom);

			if(fp==-1)
			{
				LOG_ERROR(OS_TEXT("open file error,filename: ")+fn);
				return(false);
			}

			filename=fn;
			mode=fom;
			return(true);
		}

		void CloseFile(int fp);

		void FileAccess::Close()
		{
			if(fp==-1)return;

            CloseFile(fp);

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
	}//namespace io
}//namespace hgl
