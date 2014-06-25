#ifndef HGL_IO_HASH_INPUT_STREAM_INCLUDE
#define HGL_IO_HASH_INPUT_STREAM_INCLUDE

#include<hgl/io/InputStream.h>
namespace hgl
{
	/**
	 * Hash输入流<br>
	 * Hash输入流主要用于计算输入数据的校验码，使用Hash输入流，数据将不可以重定位访问
	 */
	class HashInputStream:public InputStream														///Hash输入流
	{
	protected:

		InputStream *in;

	public:

		HashInputStream(InputStream *is)
		{
			in=is;
		}

		virtual ~HashInputStream()HGL_DEFAULT_MEMFUNC;

		virtual void	Close(){if(in)in->Close();}												///<关闭输入流

		virtual int64	Read(void *,int64)=0;													///<读取数据
		virtual int64	Peek(void *,int64){return -1;}											///<预览数据
		virtual int64	ReadFully(void *buf,int64 buf_size){return Read(buf,buf_size);}			///<充分读取,保证读取到指定长度的数据(不计算超时)

		virtual bool	CanRestart()const{return in?in->CanRestart():false;}					///<是否可以复位
		virtual bool	CanSeek()const{return in?in->CanSeek():false;}							///<是否可以定位
		virtual bool	CanSize()const{return in?in->CanSize():false;}							///<是否可以取得尺寸
		virtual bool	CanPeek()const{return false;}											///<是否可以预览数据

		virtual bool	Restart(){return in?in->Restart():false;}								///<复位访问指针
		virtual int64	Skip(int64 n){return in?in->Skip(n):-1;}								///<跳过指定字节不访问
		virtual int64	Seek(int64 n,SeekOrigin so=soBegin){return in?in->Seek(n,so):-1;}		///<移动访问指针
		virtual int64	Tell()const{return in?in->Tell():-1;}									///<返回当前访问位置
		virtual int64	GetSize()const{return in?in->GetSize():-1;}								///<取得流长度
		virtual int64	Available()const{return in?in->Available():-1;}							///<剩下的可以不受阻塞访问的字节数
	};//class HashInputStream
}//namespace hgl
#endif//HGL_IO_HASH_INPUT_STREAM_INCLUDE
