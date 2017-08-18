#ifndef HGL_THREAD_RING_BUFFER_INCLUDE
#define HGL_THREAD_RING_BUFFER_INCLUDE

#include<hgl/thread/ThreadMutex.h>
#include<hgl/io/InputStream.h>
#include<hgl/io/OutputStream.h>
namespace hgl
{
	/**
	* 多线程环形数据流,用于随时被读或写的情况以及在多线程应用中同时被读与写的情况。
	*/
	template<typename T> class RingBuffer:protected ThreadMutex																///多线程环形数据流
	{
	protected:

		T *buffer;
		int buffer_size;

		int read_pos,write_pos;

		int read_cur,write_cur;
		int read_off,write_off;
		int read_max,write_max;
		int read_count,write_count;

		bool read_peek;

	protected:

		void ClampPosition();

		int _GetReadSize()const{return write_pos-read_pos;}
		int _GetWriteSize()const{return buffer_size-(write_pos-read_pos);}

		int _SafeWriteStart();
		int _SafeReadStart(bool);

		int _Write(const T *,int);
		int _Read(T *,int,bool);

	public:

		RingBuffer(int);																			///<本类构造函数
		virtual ~RingBuffer();																		///<本类析构函数

		const int GetBufferSize()const{return buffer_size;}											///<除取缓冲区长度

		void Clear();																				///<清除整个缓冲区
		void SafeClear();																			///<安全清除整个缓冲区

	public:	//写处理函数

		int WriteStart();																			///<开始写入
		int GetWriteSize()const{return write_max;}													///<取得可写入数据长度
		int Write(const void *,int);																///<向流中写入数据
		int WriteEnd();																				///<结束写入

		int SafeTryWriteStart();																	///<安全尝试开始写入
		int SafeWriteStart();																		///<安全开始写入
		int SafeWriteEnd();																			///<安全结束写入

		int SafeGetWriteSize();																		///<安全取得可写入数据长度
		int SafeWrite(const T *,int);															    ///<安全写入，用于简单的一次性写入

	public:	//读处理函数

		int ReadStart(bool=true);																	///<开始读取
		int GetReadSize()const{return read_max;}													///<取得可读取数据长度
		int Read(T *,int,bool=true);																///<从流中读取数据
		int Peek(T *ptr,int size){return Read(ptr,size,false);}									    ///<从流中预读数据
		int Skip(int size){return Read(0,size,true);}												///<跳过流中的指定长度数据
		int ReadEnd();																				///<结束写入

		int SafeTryReadStart(bool=true);															///<安全尝试开始读取
		int SafeReadStart(bool=true);																///<安全开始读取
		int SafeReadEnd();																			///<安全结束读取

		int SafeGetReadSize();																		///<安全取得可读取数据长度
		bool SafeTryGetReadSize(int &);																///<安全尝试取得可读取数据长度
		int SafeRead(T *,int,bool=true);															///<安全读取，用于简单的一次性读取
		int SafePeek(T *ptr,int size){return SafeRead(ptr,size,false);}							    ///<安全预读数据
	};//class RingBuffer

	namespace io
	{
		class RingInputStream:public InputStream
		{
			RingBuffer<char> *rb;

		public:

			RingInputStream		(RingBuffer<char> *_rb){rb=_rb;}

			RingBuffer<char> *GetRingBuffer(){return rb;}

			int		Start		(){return rb?rb->ReadStart():-1;}
			int		End			(){return rb?rb->ReadEnd():-1;}
			int		SafeStart	(){return rb?rb->SafeReadStart():-1;}
			int		SafeEnd		(){return rb?rb->SafeReadEnd():-1;}

			void	Close		(){}

		public:

			int64	Read		(void *buf,int64 size){return rb?rb->Read(buf,size):-1;}			///<读取数据
			int64	Peek		(void *buf,int64 size){return rb?rb->Peek(buf,size):-1;}			///<预览数据

			bool	CanRestart	()const{return false;}												///<是否可以复位
			bool	CanSeek		()const{return false;}												///<是否可以定位
			bool	CanSize		()const{return false;}												///<是否可以取得尺寸
			bool	CanPeek		()const{return false;}												///<是否可以预览数据

			bool	Restart		(){return false;}													///<复位访问指针
			int64	Skip		(int64 size){return rb?rb->Skip(size):-1;}							///<跳过指定字节不访问
			int64	Seek		(int64,SeekOrigin){return -1;}										///<移动访问指针
			int64	Tell		()const{return -1;}													///<返回当前访问位置
			int64	GetSize		()const{return -1;}													///<取得流长度
			int64	Available	()const{return rb?rb->GetReadSize():-1;}							///<剩下的可以不受阻塞访问的字节数
		};//class RingInputStream

		class RingOutputStream:public OutputStream
		{
			RingBuffer<char> *rb;

		public:

			RingOutputStream	(RingBuffer<char> *_rb){rb=_rb;}

			RingBuffer<char> *GetRingBuffer(){return rb;}

			int		Start		(){return rb?rb->WriteStart():-1;}
			int		End			(){return rb?rb->WriteEnd():-1;}
			int		SafeStart	(){return rb?rb->SafeWriteStart():-1;}
			int		SafeEnd		(){return rb?rb->SafeWriteEnd():-1;}

			void	Close		(){}

		public:

			int64	Write		(const void *buf,int64 size){return rb?rb->Write(buf,size):-1;}		///<写入数据

			bool	CanRestart	()const{return false;};												///<是否可以复位
			bool	CanSeek		()const{return false;};												///<是否可以定位
			bool	CanSize		()const{return false;}												///<是否可以取得尺寸

			bool	Restart		(){return false;}													///<复位访问指针
			int64	Seek		(int64,SeekOrigin){return -1;}										///<移动访问指针
			int64	Tell		()const{return -1;}													///<返回当前访问位置
			int64	GetSize		()const{return -1;}													///<取得流长度
			int64	Available	()const{return rb?rb->GetWriteSize():-1;}							///<剩下的可以不受阻塞写入的字节数
		};//class RingOutputStream
	}//namespace io

	class RingBufferRead
	{
		RingBuffer<char> *rb;
		int size;

	public:

		RingBufferRead(RingBuffer<char> *_rb)
		{
			rb=_rb;

			size=rb->ReadStart();
		}

		RingBufferRead(io::RingInputStream *ris)
		{
			rb=ris->GetRingBuffer();

			size=rb->ReadStart();
		}

		~RingBufferRead()
		{
			Unlock();
		}

		void Unlock()
		{
			if(size>0)
			{
				rb->ReadEnd();
				size=0;
			}
		}

		int GetSize() const
		{
			return size;
		}
	};//class RingBufferRead

	class RingBufferSafeRead
	{
		RingBuffer<char> *rb;
		int size;

	public:

		RingBufferSafeRead(RingBuffer<char> *_rb)
		{
			rb=_rb;
			size=-1;
		}

		RingBufferSafeRead(io::RingInputStream *ris)
		{
			rb=ris->GetRingBuffer();
			size=-1;
		}

		int Start()
		{
			size=rb->SafeReadStart();
			return size;
		}

		int TryStart()
		{
			size=rb->SafeTryReadStart();
			return size;
		}

		int GetSize()
		{
			size=rb->GetReadSize();
			return size;
		}

		~RingBufferSafeRead()
		{
			Unlock();
		}

		void Unlock()
		{
			if(size>0)
			{
				rb->SafeReadEnd();
				size=0;
			}
		}
	};//class RingBufferSafeRead

	class RingBufferWrite
	{
		RingBuffer<char> *rb;
		int size;

	public:

		RingBufferWrite(RingBuffer<char> *_rb)
		{
			rb=_rb;

			size=rb->WriteStart();
		}

		RingBufferWrite(io::RingOutputStream *ros)
		{
			rb=ros->GetRingBuffer();

			size=rb->WriteStart();
		}

		~RingBufferWrite()
		{
			Unlock();
		}

		void Unlock()
		{
			if(size>0)
			{
				rb->WriteEnd();
				size=0;
			}
		}

		int GetSize() const
		{
			return size;
		}
	};//class RingBufferWrite

	class RingBufferSafeWrite
	{
		RingBuffer<char> *rb;
		int size;

	public:

		RingBufferSafeWrite(RingBuffer<char> *_rb)
		{
			rb=_rb;

			size=-1;
		}

		RingBufferSafeWrite(io::RingOutputStream *ros)
		{
			rb=ros->GetRingBuffer();
		}

		int Start()
		{
			size=rb->SafeWriteStart();
			return(size);
		}

		int TryStart()
		{
			size=rb->SafeTryWriteStart();
			return(size);
		}

		~RingBufferSafeWrite()
		{
			Unlock();
		}

		int Unlock()
		{
			int result=size;

			if(size>0)
			{
				rb->SafeWriteEnd();
				size=0;
			}

			return result;
		}
	};//class RingBufferSafeWrite
}//namespace hgl
#include<hgl/thread/RingBuffer.cpp>
#include<hgl/thread/RingBufferRead.cpp>
#include<hgl/thread/RingBufferWrite.cpp>
#endif//HGL_THREAD_RING_BUFFER_INCLUDE
