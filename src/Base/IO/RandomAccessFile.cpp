#include<hgl/io/RandomAccessFile.h>
#include<hgl/io/FileInputStream.h>
#include<hgl/io/FileOutputStream.h>

namespace hgl
{
	namespace io
	{
		RandomAccessFile::RandomAccessFile()
		{
			file=nullptr;
		}

		RandomAccessFile::RandomAccessFile(SharedPtr<FileAccess> &f)
		{
			file=f;
		}

		RandomAccessFile::~RandomAccessFile()
		{
			Close();
		}

		bool RandomAccessFile::Open(const OSString &filename)
		{
			Close();

			file=new FileAccess();

			if(!file->Open(filename,fomReadWrite))
			{
				file=nullptr;
				return(false);
			}

			return(true);
		}

		void RandomAccessFile::Close()
		{
			if(file)
				file=nullptr;
		}

		int64	RandomAccessFile::Read		(void *buf,int64 size)					{return file?file->Read(buf,size):-1;}
		int64	RandomAccessFile::Write		(const void *buf,int64 size)			{return file?file->Write(buf,size):-1;}

		bool	RandomAccessFile::CanSeek	()const									{return file?file->CanSeek():false;}
		bool	RandomAccessFile::CanRestart()const									{return file?file->CanRestart():false;}
		bool	RandomAccessFile::CanSize	()const									{return file?file->CanSize():false;}

		int64	RandomAccessFile::Tell		()const									{return file?file->Tell():-1;}
		int64	RandomAccessFile::GetSize	()										{return file?file->GetSize():-1;}
		bool	RandomAccessFile::Restart	()										{return file?file->Restart():false;}
		int64	RandomAccessFile::Seek		(int64 off,SeekOrigin so)				{return file?file->Seek(off,so):-1;}

		int64	RandomAccessFile::Read		(int64 off,void *buf,int64 size)		{return file?file->Read(off,buf,size):-1;}
		int64	RandomAccessFile::Write		(int64 off,const void *buf,int64 size)	{return file?file->Write(off,buf,size):-1;}
	}//namespace io
}//namespace hgl
