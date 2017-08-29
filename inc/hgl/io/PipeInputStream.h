#ifndef HGL_IO_PIPE_INPUT_STREAM_INCLUDE
#define HGL_IO_PIPE_INPUT_STREAM_INCLUDE

#include<hgl/io/InputStream.h>
#include<hgl/io/SeekAccess.h>
namespace hgl
{
	namespace io
	{
        // 根据网络资料，本机通信pipes/socket，pipes快大概10%-12%左右。

		/**
		 * 管道输入流
		 */
		class PipeInputStream:public InputStream
		{
			int pipe_fd;

		public:

			PipeInputStream(){pipe_fd=-1;}
			PipeInputStream(int fd){pipe_fd=fd;}
			~PipeInputStream();

			void Init(int fd){pipe_fd=fd;}
			void Close(){pipe_fd=-1;}

			int64	Read(void *,int64);														///<读取数据
			int64	Peek(void *,int64){return -1;}											///<预览数据
			int64	ReadFully(void *buf,int64 buf_size){return Read(buf,buf_size);}			///<充分读取,保证读取到指定长度的数据(不计算超时)

			bool	CanRestart()const{return false;}										///<是否可以复位
			bool	CanSeek()const{return false;}											///<是否可以定位
			bool	CanSize()const{return false;}											///<是否可以取得尺寸
			bool	CanPeek()const{return false;}											///<是否可以预览数据

			bool	Restart(){return false;}												///<复位访问指针
			int64	Skip(int64)=0;															///<跳过指定字节不访问
			int64	Seek(int64,SeekOrigin=soBegin){return -1;}								///<移动访问指针
			int64	Tell()const{return -1;}													///<返回当前访问位置
			int64	GetSize()const{return -1;}												///<取得流长度
			int64	Available()const{return -1;}											///<剩下的可以不受阻塞访问的字节数
		};//class PipeInputStream
	}//namespace io
}//namespace hgl
#endif//HGL_IO_PIPE_INPUT_STREAM_INCLUDE
