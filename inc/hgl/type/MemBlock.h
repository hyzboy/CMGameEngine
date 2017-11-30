#ifndef HGL_MEM_BLOCK_INCLUDE
#define HGL_MEM_BLOCK_INCLUDE

#include<hgl/type/DataType.h>
#include<hgl/io/FileInputStream.h>
#include<hgl/FileSystem.h>
#include<hgl/thread/ThreadMutex.h>
namespace hgl
{
	/**
	 * 内存块模版
	 */
	template<typename T> class MemBlock
	{
	protected:

		T *buf;
		size_t cur_size;
		size_t buf_size;

	public:

				size_t GetLength	()const{return cur_size;}										///<取得内存块长度(注：非字节数)
		const	size_t GetMaxLength	()const{return buf_size;}										///<取得内存块最大长度(注：非字节数)
		const	size_t GetBytes		()const{return cur_size*sizeof(T);}								///<取得内存块字节数
		const	size_t GetMaxBytes	()const{return buf_size*sizeof(T);}								///<取得内存块最大字节数

		void SetLength(size_t size)																	///<设置内存块长度(注：非字节数)
		{
			if(size<=buf_size)
			{
				cur_size=size;
				return;
			}

			buf_size=power_to_2(size);

			if(!buf)
				buf=(T *)hgl_malloc(buf_size*sizeof(T));
			else
				buf=(T *)hgl_realloc(buf,buf_size*sizeof(T));

			cur_size=size;
		}

		void AddLength(size_t size)
		{
			SetLength(cur_size+size);
		}

	public:

		MemBlock()
		{
			buf=0;
			cur_size=0;
			buf_size=0;
		}

		MemBlock(size_t size)
		{
			if(size<=0)
				buf=0;
			else
				buf=(T *)hgl_malloc(size*sizeof(T));

			if(buf)
			{
				cur_size=size;
				buf_size=size;
			}
			else
			{
				cur_size=0;
				buf_size=0;
			}
		}

		virtual ~MemBlock()
		{
			Clear();
		}

		void Clear()
		{
			if(buf)
				hgl_free(buf);

			cur_size=0;
			buf_size=0;
		}

		void ClearData()
		{
			cur_size=0;
		}

		void Zero()
		{
			if(buf)
				memset(buf,0,buf_size);
		}

		/**
		 * 设置数据,请保证数据使用hgl_malloc分配，否则会因为释放函数不配对出现错误
		 */
		void SetData(T *d,int s)
		{
			Clear();

			buf=d;
			buf_size=s;
			cur_size=s;
		}

		/**
		 * 解除数据关联
		 */
		void Unlink()
		{
			buf=nullptr;
			cur_size=0;
			buf_size=0;
		}

		/**
		 * 复制内存块中的数据
		 * @param d 复制出来的缓冲区指针
		 * @param s 要复制出来的数据个数
		 */
		void CopyData(T *d,int s)
		{
			SetLength(s);
			memcpy(buf,d,s*sizeof(T));
		}

		T *data()const{return buf;}
		T *GetData()const{return buf;}

		size_t length()const{return cur_size;}
		size_t GetCount()const{return cur_size;}

		size_t bytes()const
		{
			return cur_size*sizeof(T);
		}

		operator T *()const
		{
			return buf;
		}

		T *operator ->()const
		{
			return buf;
		}

		T &operator[](int n)
		{
			return buf+n;
		}
	};//template<typename T> class MemBlock

	/**
	 * 加载一个文件到内存块类中
	 */
	template<typename T> MemBlock<T> *LoadFileToMemBlock(const OSString &filename)
	{
		io::FileInputStream fis;

		if(!fis.Open(filename))return(nullptr);

		const size_t file_size	=fis.GetSize();
		const size_t size		=(file_size+sizeof(T)-1)/sizeof(T);

		MemBlock<T> *mb=new MemBlock<T>(size);

		fis.Read(mb->data(),file_size);

		return(mb);
	}

	/**
	 * 保存一个内存块到文件
	 */
	template<typename T> bool SaveMemBlockToFile(const OSString &filename,const MemBlock<T> &mb)
	{
		const size_t size=mb.bytes();

		if(size<=0)return(true);

		return(SaveMemoryToFile(filename,mb.data(),mb.bytes())==size);
	}
}//namespace hgl
#endif//HGL_MEM_BLOCK_INCLUDE
