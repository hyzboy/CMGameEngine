#ifndef HGL_IO_MEMORY_INPUT_STREAM_INCLUDE
#define HGL_IO_MEMORY_INPUT_STREAM_INCLUDE

#include<hgl/MemBlock.h>
#include<hgl/io/InputStream.h>
namespace hgl
{
	namespace io
	{
		/**
		 * 内存数据输入流，从内存块中取得数据。
		 */
		class MemoryInputStream:public InputStream													///内存数据输入流
		{
		protected:

			const uint8 *buf;
			size_t buf_size;
			size_t cur_pos;

		public:

			MemoryInputStream()
			{
				buf=0;

				buf_size=0;
				cur_pos=0;
			}

			virtual ~MemoryInputStream()
			{
			}

			/**
			 * 关联一个数据区到当前输入流
			 * @param data 数据指针
			 * @param size 数据长度字节数
			 * @return 是否成功
			 */
			bool Link(const void *data,const size_t size)
			{
				if(!data)
					return(false);

				buf=(uint8 *)data;
				buf_size=size;
				cur_pos=0;

				return(true);
			}

			/**
			 * 更新关联的数据区长度，不复位cur_pos
			 * @param data 数据指针
			 * @param size 数据长度字节数
			 * @return 是否成功
			 */
			bool Update(void *data,size_t size)
			{
				if(!data)
					return(false);

				buf=(uint8 *)data;
				buf_size=size;

				return(true);
			}

			void Unlink()
			{
				buf=0;
			}

			void Close()
			{
				Unlink();
			}

			int64 Read(void *data,int64 size)
			{
				const int result=Peek(data,size);

				if(result>0)
					cur_pos+=result;

				return result;
			}

			int64 Peek(void *data,int64 size)
			{
				if(!buf||!data||size<0)
					return(-1);

				if(cur_pos+size>buf_size)
					size=buf_size-cur_pos;

				if(size<=0)
					return(0);

				memcpy(data,buf+cur_pos,size);

				return size;
			}

			bool	CanRestart()const{return true;}
			bool	CanSeek()const{return true;}
			bool	CanSize()const{return true;}
			bool	CanPeek()const{return true;}

			bool	Restart()
			{
				cur_pos=0;
				return(true);
			}

			int64	Skip(int64 bytes)
			{
				return Seek(bytes,soCurrent);
			}

			int64	Seek(int64 off,SeekOrigin so)
			{
				if(!CanSeek())return(-1);

				if(so==soCurrent)
				{
					off+=cur_pos;
				}
				else
				if(so==soEnd)
				{
					off+=buf_size;
				}

				if(off<0||off>=buf_size)
					return(-1);

				cur_pos=off;
				return cur_pos;
			}

			int64	Tell()const
			{
				if(!buf)return(-1);
				return cur_pos;
			}

			int64	GetSize()const
			{
				if(!buf)return(-1);
				return buf_size;
			}

			int64	Available()const
			{
				if(!buf)return(-1);
				return buf_size-cur_pos;
			}
		};//class MemoryInputStream
	}//namespace io
}//namespace hgl
#endif//HGL_IO_MEMORY_INPUT_STREAM_INCLUDE
