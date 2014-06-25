#ifndef HGL_IO_OUTPUT_STREAM_INCLUDE
#define HGL_IO_OUTPUT_STREAM_INCLUDE

#include<hgl/type/DataType.h>
#include<hgl/io/SeekAccess.h>
namespace hgl
{
	namespace io
	{
		/**
		* 数据输出流基类
		*/
		class OutputStream:public SeekAccess														///数据输出流基类
		{
		public:

			virtual ~OutputStream()HGL_DEFAULT_MEMFUNC;

			virtual void	Close()=0;																///<关闭输出流

			virtual int64	Write(const void *,int64)=0;											///<写入数据
			virtual int64	WriteFully(const void *buf,int64 buf_size){return Write(buf,buf_size);}	///<充分写入,保证完整写入指定长度的数据

			virtual bool	CanRestart()const=0;													///<是否可以复位
			virtual bool	CanSeek()const=0;														///<是否可以定位
			virtual bool	CanSize()const=0;														///<是否可以取得尺寸

			virtual bool	Restart()=0;															///<复位访问指针
			virtual int64	Seek(int64,SeekOrigin=soBegin)=0;										///<移动访问指针
			virtual int64	Tell()const=0;															///<返回当前访问位置
			virtual int64	GetSize()const=0;														///<取得流长度
			virtual int64	Available()const=0;														///<剩下的可以不受阻塞写入的字节数
		};//class OutputStream
	}//namespace io
}//namespace hgl
#endif//HGL_IO_OUTPUT_STREAM_INCLUDE
