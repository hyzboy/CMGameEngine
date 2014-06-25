#ifndef HGL_IO_RANDOM_ACCESS_FILE_INCLUDE
#define HGL_IO_RANDOM_ACCESS_FILE_INCLUDE

#include<hgl/io/InputStream.h>
#include<hgl/io/OutputStream.h>
#include<hgl/io/FileAccess.h>
namespace hgl
{
	namespace io
	{
		/**
		* 随机文件访问</p>
		* 可同时被DataInputStream/DataOutputStream包含引用，但需注意他们将共用一个访问指针
		*/
		class RandomAccessFile:public InputStream,public OutputStream
		{
		protected:

			SharedPtr<FileAccess> file;																///<文件访问指针

		public:

			RandomAccessFile();
			RandomAccessFile(SharedPtr<FileAccess> &);
			~RandomAccessFile();

			virtual bool	Open(const OSString &);													///<打开文件
			virtual void	Close();																///<关闭文件

			virtual int64	Read(void *,int64);														///<读取数据
			virtual int64	Write(const void *,int64);												///<写入数据

			virtual int64	Read(int64,void *,int64);												///<在指定位置读取指定长度的数据
			virtual int64	Write(int64,const void *,int64);										///<在指定位置写入指定长度的数据

			virtual bool	CanSeek()const;															///<是否可移动访问指针
			virtual bool	CanRestart()const;														///<是否可复位访问
			virtual bool	CanSize()const;															///<是否可访问文件长度

			virtual int64	Tell()const;															///<取当前位置
			virtual int64	GetSize();																///<取得文件长度
			virtual bool	Restart();																///<复位访问指针
			virtual int64	Seek(int64,SeekOrigin=soBegin);											///<移动访问指针
		};//class RandomAccessFile
	}//namespace io
}//namespace hgl
#endif//HGL_IO_RANDOM_ACCESS_FILE_INCLUDE
