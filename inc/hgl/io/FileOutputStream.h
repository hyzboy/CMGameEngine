#ifndef HGL_IO_FILE_OUTPUT_STREAM_INCLUDE
#define HGL_IO_FILE_OUTPUT_STREAM_INCLUDE

#include<hgl/io/OutputStream.h>
#include<hgl/io/FileAccess.h>
namespace hgl
{
	namespace io
	{
		/**
		* 文件输出流类
		*/
		class FileOutputStream:public OutputStream													///文件输出流类
		{
		protected:

			SharedPtr<FileAccess> file;																///<文件访问指针

			virtual bool Open(const OSString &,int);

		public:

			FileOutputStream();
			FileOutputStream(SharedPtr<FileAccess> &);
			virtual ~FileOutputStream();

			virtual bool	Open(const OSString &fn){return Open(fn,fomOnlyWrite);}					///<打开文件
			virtual bool	Create(const OSString &fn){return Open(fn,fomCreate);}					///<创建文件，如存在创建失败
			virtual bool	CreateTrunc(const OSString &fn){return Open(fn,fomCreateTrunc);}		///<创建文件，如存在则抹去
			virtual bool	OpenAppend(const OSString &fn){return Open(fn,fomAppend);}				///<打开文件，追加模式

			virtual void	Close();																///<关闭文件

			virtual int64	Write(const void *,int64);												///<写入数据

			virtual bool	CanSeek()const;															///<是否可移动访问指针
			virtual bool	CanRestart()const;														///<是否可复位访问
			virtual bool	CanSize()const;															///<是否可访问文件长度

			virtual int64	Tell()const;															///<取当前位置
			virtual int64	GetSize()const;															///<取得文件长度
			virtual bool	Restart();																///<复位访问指针
			virtual int64	Seek(int64,SeekOrigin=soBegin);											///<移动访问指针
			virtual int64	Available()const{return -1;}											///<可不受影响写入的字节数

			virtual int64	Write(int64,const void *,int64);										///<在指定位置写入指定长度的数据
		};//class FileOutputStream

		class OpenFileOutputStream
		{
			FileOutputStream *fos;

		public:

			OpenFileOutputStream(const OSString &filename)
			{
				fos=new FileOutputStream();

				fos->Open(filename);
			}

			~OpenFileOutputStream()
			{
				if(fos)delete fos;
			}

			operator FileOutputStream *(){return fos;}
			FileOutputStream *operator ->(){return fos;}
		};//class OpenFileInputStream
	}//namespace io
}//namespace hgl
#endif//HGL_IO_FILE_OUTPUT_STREAM_INCLUDE
